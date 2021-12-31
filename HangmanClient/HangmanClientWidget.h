#pragma once

#include <LibGUI/ImageWidget.h>
#include <LibGUI/TextBox.h>
#include <LibGUI/Button.h>
#include <LibGUI/Label.h>

#include <LibWebSocket/WebSocket.h>

#include <array>

enum class GameType {
    LocalSolo,
    LocalMultiplayer,
    RemoteMultiplayer,
};

class HangmanClientWidget final : public GUI::Widget {
    C_OBJECT(HangmanClientWidget);

private:
    HangmanClientWidget();

    void start_game(GameType type);
    void lock_guess_word();

    GameType m_game_type;
    String m_game_id;
    String m_me;

    RefPtr<Core::Timer> m_ping_timer;
    RefPtr<Core::Timer> m_guess_word_unlock_timer;

    RefPtr<GUI::Widget> m_welcome_widget;
    RefPtr<GUI::ImageWidget> m_websocket_icon;
    RefPtr<GUI::UrlBox> m_websocket_url_box;
    RefPtr<GUI::Button> m_websocket_connect_button;
    RefPtr<GUI::Button> m_websocket_disconnect_button;

    RefPtr<GUI::Button> m_play_local_solo_button;
    RefPtr<GUI::Button> m_play_local_multiplayer_button;
    RefPtr<GUI::Button> m_play_remote_multiplayer_create_button;
    RefPtr<GUI::Button> m_play_remote_multiplayer_join_button;

    RefPtr<GUI::Widget> m_waiting_for_partner_widget;
    RefPtr<GUI::ImageWidget> m_waiting_for_partner_icon;
    RefPtr<GUI::Label> m_waiting_for_partner_label;
    RefPtr<GUI::Button> m_copy_game_id_button;

    RefPtr<GUI::Widget> m_game_widget;

    RefPtr<GUI::Label> m_turn;
    RefPtr<GUI::Label> m_word;
    RefPtr<GUI::Label> m_lives;

    std::array<RefPtr<GUI::Button>, 26> m_letters;

    RefPtr<GUI::TextBox> m_guess_word_text_box;
    RefPtr<GUI::Button> m_guess_word_button;

    RefPtr<GUI::Button> m_new_game_button;
    RefPtr<GUI::Button> m_leave_game_button;

    RefPtr<WebSocket::WebSocket> m_websocket;
};
