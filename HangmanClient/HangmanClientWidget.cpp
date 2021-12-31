#include <serenity.h>

#include <LibGUI/MessageBox.h>
#include <LibGUI/TextEditor.h>
#include <LibGUI/InputBox.h>

#include <Applications/HangmanClient/HangmanClientWidgetGML.h>
#include "HangmanClientWidget.h"

#include <string>

HangmanClientWidget::HangmanClientWidget()
{
    load_from_gml(hangman_client_gml);

    m_welcome_widget = *find_descendant_of_type_named<GUI::Widget>("welcome");

    m_websocket_icon = *find_descendant_of_type_named<GUI::ImageWidget>("websocket_icon");
    m_websocket_url_box = *find_descendant_of_type_named<GUI::UrlBox>("websocket_url");
    m_websocket_connect_button = *find_descendant_of_type_named<GUI::Button>("websocket_connect");
    m_websocket_disconnect_button = *find_descendant_of_type_named<GUI::Button>("websocket_disconnect");

    m_play_local_solo_button = *find_descendant_of_type_named<GUI::Button>("play_local_solo");
    m_play_local_multiplayer_button = *find_descendant_of_type_named<GUI::Button>("play_local_multiplayer");
    m_play_remote_multiplayer_create_button = *find_descendant_of_type_named<GUI::Button>("play_remote_multiplayer_create");
    m_play_remote_multiplayer_join_button = *find_descendant_of_type_named<GUI::Button>("play_remote_multiplayer_join");

    m_waiting_for_partner_widget = *find_descendant_of_type_named<GUI::Widget>("waiting_for_partner");
    m_waiting_for_partner_icon = *find_descendant_of_type_named<GUI::ImageWidget>("waiting_for_partner_icon");
    m_waiting_for_partner_label = *find_descendant_of_type_named<GUI::Label>("waiting_for_partner_label");
    m_copy_game_id_button = *find_descendant_of_type_named<GUI::Button>("copy_game_id_button");

    m_game_widget = *find_descendant_of_type_named<GUI::Widget>("game");

    m_turn = *find_descendant_of_type_named<GUI::Label>("turn");
    m_word = *find_descendant_of_type_named<GUI::Label>("word");
    m_lives = *find_descendant_of_type_named<GUI::Label>("lives");

    m_guess_word_text_box = *find_descendant_of_type_named<GUI::TextBox>("guess_word_text_box");
    m_guess_word_button = *find_descendant_of_type_named<GUI::Button>("guess_word_button");

    m_new_game_button = *find_descendant_of_type_named<GUI::Button>("new_game");
    m_leave_game_button = *find_descendant_of_type_named<GUI::Button>("leave_game");

    for (int i = 0; auto letter : String("abcdefghijklmnopqrstuvwxyz")) {
        m_letters[i] = *find_descendant_of_type_named<GUI::Button>(std::string(1, letter).c_str());
        m_letters[i]->on_click = [this, i, letter](auto) {
            m_websocket->send(WebSocket::Message(String::formatted("{{\"action\":\"guess_letter\", \"letter\":\"{}\"}}",
                static_cast<char>(toupper(letter)))));
        };
        i++;
    }

    m_websocket_icon->load_from_file("/res/icons/32x32/network.png");
    m_waiting_for_partner_icon->load_from_file("/res/icons/32x32/search-engine.png");

    m_websocket_connect_button->on_click = [this](auto) {
        m_websocket_url_box->set_enabled(false);

        m_websocket = WebSocket::WebSocket::create(WebSocket::ConnectionInfo(m_websocket_url_box->text()));

        m_websocket->on_open = [this]() {
            m_websocket_connect_button->set_visible(false);
            m_websocket_disconnect_button->set_visible(true);

            m_play_local_solo_button->set_enabled(true);
            m_play_local_multiplayer_button->set_enabled(true);
            m_play_remote_multiplayer_create_button->set_enabled(true);
            m_play_remote_multiplayer_join_button->set_enabled(true);

            m_websocket_url_box->set_focus(false);

            m_ping_timer = Core::Timer::create_repeating(5'000, [this]() {
                m_websocket->send(WebSocket::Message("{\"action\":\"ping\"}"));
            });
            m_ping_timer->start();
        };
        m_websocket->on_message = [this](WebSocket::Message incoming_message) {
            auto data = JsonValue::from_string(String(incoming_message.data().bytes())).value().as_object();

            auto message = data.get("message").to_string();
            if (message == "update") {
                m_welcome_widget->set_visible(false);
                m_waiting_for_partner_widget->set_visible(false);
                m_game_widget->set_visible(true);

                if (data.get("guess_word_locked").to_bool()) {
                    m_guess_word_text_box->set_enabled(false);
                    m_guess_word_button->set_enabled(false);
                }

                if (m_game_type != GameType::LocalSolo) {
                    String player = data.get("player").to_string();
                    if (player == m_me)
                        m_turn->set_text("Your turn");
                    else
                        m_turn->set_text("Partner's turn");
                    if (m_game_type == GameType::RemoteMultiplayer) {
                        for (auto button : m_letters)
                            button->set_enabled(player == m_me);
                        if (m_guess_word_unlock_timer)
                            m_guess_word_unlock_timer->stop();
                        m_guess_word_text_box->set_enabled(false);
                        m_guess_word_button->set_enabled(false);
                    }
                } else {
                    m_turn->set_text("");
                }

                String word = data.get("word").to_string();
                m_word->set_text(word);

                int lives = data.get("lives").to_int();
                if (word.contains('_')) {
                    m_lives->set_text(String::formatted("Lives Remaining: {}", lives));
                    data.get("letters").as_array().for_each([this](auto letter) {
                        m_letters["ABCDEFGHIJKLMNOPQRSTUVWXYZ"sv.find(letter.to_string()).value()]->set_enabled(false);
                    });
                } else {
                    m_turn->set_text("Game over!");

                    if (lives > 0)
                        m_lives->set_text("You win!");
                    else
                        m_lives->set_text("You lose!");

                    for (auto button : m_letters)
                        button->set_enabled(false);

                    if (m_guess_word_unlock_timer)
                        m_guess_word_unlock_timer->stop();

                    m_guess_word_text_box->set_enabled(false);
                    m_guess_word_button->set_enabled(false);

                    m_new_game_button->set_enabled(true);
                }
            } else if (message == "wait_for_partner") {
                m_game_id = data.get("game_id").to_string();
                m_welcome_widget->set_visible(false);
                m_waiting_for_partner_widget->set_visible(true);
                m_waiting_for_partner_label->set_text(String::formatted("Waiting for Partner\nGame ID: {}", m_game_id));
            } else if (message == "cannot_join_game") {
                GUI::MessageBox::show(window(), "You may have entered an invalid Game ID.", "Cannot Join Game", GUI::MessageBox::Type::Warning);
            } else if (message == "lock_guess_word") {
                lock_guess_word();
            }
        };

        m_websocket->on_close = [this]([[maybe_unused]] u16, [[maybe_unused]] String, [[maybe_unused]] bool) {
            if (m_ping_timer)
                m_ping_timer->stop();
            if (m_guess_word_unlock_timer)
                m_guess_word_unlock_timer->stop();

            m_welcome_widget->set_visible(true);
            m_game_widget->set_visible(false);

            m_websocket_url_box->set_enabled(true);
            m_websocket_url_box->set_text("");

            m_websocket_connect_button->set_visible(true);
            m_websocket_disconnect_button->set_visible(false);

            m_play_local_solo_button->set_enabled(false);
            m_play_local_multiplayer_button->set_enabled(false);
            m_play_remote_multiplayer_create_button->set_enabled(false);
            m_play_remote_multiplayer_join_button->set_enabled(false);
        };
        m_websocket->on_error = [this]([[maybe_unused]] WebSocket::WebSocket::Error) {
            GUI::MessageBox::show(window(), "An error occurred.", "Error", GUI::MessageBox::Type::Error);
        };

        m_websocket->start();
    };

    m_websocket_disconnect_button->on_click = [this](auto) {
        m_websocket->close();
    };

    m_copy_game_id_button->on_click = [this](auto) {
        GUI::Clipboard::the().set_plain_text(m_game_id);
    };

    m_play_local_solo_button->on_click = [this](auto) {
        m_me = "a";
        start_game(GameType::LocalSolo);
    };
    m_play_local_multiplayer_button->on_click = [this](auto) {
        m_me = "a";
        start_game(GameType::LocalMultiplayer);
    };
    m_play_remote_multiplayer_create_button->on_click = [this](auto) {
        m_me = "a";
        start_game(GameType::RemoteMultiplayer);
    };
    m_play_remote_multiplayer_join_button->on_click = [this](auto) {
        String game_id;
        if (GUI::InputBox::show(window(), game_id, "Game ID:", "Remote Multiplayer") == GUI::InputBox::ExecOK && !game_id.is_empty()) {
            m_game_type = GameType::RemoteMultiplayer;
            m_me = "b";
            m_websocket->send(WebSocket::Message(String::formatted("{{\"action\":\"join_game\",\"game_id\":\"{}\"}}",
                game_id)));
        }
    };

    m_guess_word_button->on_click = [this](auto) {
        String guess = m_guess_word_text_box->text().to_uppercase();
        for (auto letter : guess) {
            if (!String("ABCDEFGHIJKLMNOPQRSTUVWXYZ").contains(letter))
                return;
        }
        m_websocket->send(WebSocket::Message(String::formatted("{{\"action\":\"guess_word\",\"word\":\"{}\"}}",
            guess)));

        m_guess_word_text_box->set_text("");

        lock_guess_word();
    };

    m_new_game_button->on_click = [this](auto) {
        m_websocket->send(WebSocket::Message("{\"action\":\"new_game\"}"));
        if (m_guess_word_unlock_timer)
            m_guess_word_unlock_timer->stop();
        m_guess_word_text_box->set_enabled(true);
        m_guess_word_button->set_enabled(true);
        m_new_game_button->set_enabled(false);
        for (auto button : m_letters)
            button->set_enabled(true);
    };

    m_leave_game_button->on_click = [this](auto) {
        m_websocket->close();
    };
}
void HangmanClientWidget::start_game(GameType type)
{
    m_game_type = type;

    m_websocket->send(WebSocket::Message(String::formatted("{{\"action\":\"create_game\", \"game_type\":\"{}\"}}",
        type == GameType::RemoteMultiplayer ? "remote" : "local")));
}
void HangmanClientWidget::lock_guess_word()
{
    if (m_guess_word_unlock_timer)
        m_guess_word_unlock_timer->stop();

    m_guess_word_text_box->set_enabled(false);
    m_guess_word_button->set_enabled(false);

    m_guess_word_unlock_timer = Core::Timer::create_single_shot(5'000, [this]() {
        m_guess_word_text_box->set_enabled(true);
        m_guess_word_button->set_enabled(true);
    });

    m_guess_word_unlock_timer->start();
}
