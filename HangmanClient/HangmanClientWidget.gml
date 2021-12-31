@GUI::Widget {
    fill_with_background_color: true
    layout: @GUI::VerticalBoxLayout

    @GUI::Widget {
        name: "welcome"
        layout: @GUI::VerticalBoxLayout {
            margins: [20]
        }

        @GUI::Widget {
            layout: @GUI::HorizontalBoxLayout {
                spacing: 10
            }

            @GUI::ImageWidget {
                name: "websocket_icon"
            }

            @GUI::Widget {
                max_height: 50
                layout: @GUI::VerticalBoxLayout

                @GUI::Label {
                    text: "Enter the URL of the WebSocket you wish to connect to"
                    text_alignment: "CenterLeft"
                }

                @GUI::Widget {
                    layout: @GUI::HorizontalBoxLayout {
                        spacing: 10
                    }

                    @GUI::UrlBox {
                        name: "websocket_url"
                        placeholder: "ws://127.0.0.1:5522/ws"
                    }

                    @GUI::Button {
                        name: "websocket_connect"
                        text: "Connect"
                        fixed_width: 100
                    }
                    @GUI::Button {
                        name: "websocket_disconnect"
                        text: "Disconnect"
                        fixed_width: 100
                        visible: false
                    }
                }
            }
        }

        @GUI::Widget {
            layout: @GUI::VerticalBoxLayout {
                spacing: 10
            }

            @GUI::Button {
                name: "play_local_solo"
                text: "Local Solo"
                enabled: "false"
            }
            @GUI::Button {
                name: "play_local_multiplayer"
                text: "Local Multiplayer"
                enabled: "false"
            }
            @GUI::Button {
                name: "play_remote_multiplayer_create"
                text: "Remote Multiplayer — Create"
                enabled: "false"
            }
            @GUI::Button {
                name: "play_remote_multiplayer_join"
                text: "Remote Multiplayer — Join"
                enabled: "false"
            }
        }
    }

    @GUI::Widget {
        name: "waiting_for_partner"
        visible: false
        layout: @GUI::VerticalBoxLayout {
            margins: [25]
        }

        @GUI::ImageWidget {
            name: "waiting_for_partner_icon"
        }

        @GUI::Label {
            name: "waiting_for_partner_label"
        }

        @GUI::Button {
            name: "copy_game_id_button"
            text: "Copy Game ID"
        }
    }

    @GUI::Widget {
        name: "game"
        visible: false
        layout: @GUI::VerticalBoxLayout {
            margins: [10]
        }

        @GUI::Label {
            name: "turn"
            text: ""
        }

        @GUI::Label {
            name: "word"
            text: ""
        }

        @GUI::Label {
            name: "lives"
            text: ""
        }

        @GUI::Frame {
            layout: @GUI::VerticalBoxLayout {
                margins: [5]
                spacing: 0
            }

            @GUI::Widget {
                layout: @GUI::HorizontalBoxLayout

                @GUI::Button {
                    name: "a"
                    text: "A"
                }
                @GUI::Button {
                    name: "b"
                    text: "B"
                }
                @GUI::Button {
                    name: "c"
                    text: "C"
                }
                @GUI::Button {
                    name: "d"
                    text: "D"
                }
                @GUI::Button {
                    name: "e"
                    text: "E"
                }
                @GUI::Button {
                    name: "f"
                    text: "F"
                }
                @GUI::Button {
                    name: "g"
                    text: "G"
                }
                @GUI::Button {
                    name: "h"
                    text: "H"
                }
                @GUI::Button {
                    name: "i"
                    text: "I"
                }
                @GUI::Button {
                    name: "j"
                    text: "J"
                }
                @GUI::Button {
                    name: "k"
                    text: "K"
                }
                @GUI::Button {
                    name: "l"
                    text: "L"
                }
                @GUI::Button {
                    name: "m"
                    text: "M"
                }
            }
            @GUI::Widget {
                layout: @GUI::HorizontalBoxLayout

                @GUI::Button {
                    name: "n"
                    text: "N"
                }
                @GUI::Button {
                    name: "o"
                    text: "O"
                }
                @GUI::Button {
                    name: "p"
                    text: "P"
                }
                @GUI::Button {
                    name: "q"
                    text: "Q"
                }
                @GUI::Button {
                    name: "r"
                    text: "R"
                }
                @GUI::Button {
                    name: "s"
                    text: "S"
                }
                @GUI::Button {
                    name: "t"
                    text: "T"
                }
                @GUI::Button {
                    name: "u"
                    text: "U"
                }
                @GUI::Button {
                    name: "v"
                    text: "V"
                }
                @GUI::Button {
                    name: "w"
                    text: "W"
                }
                @GUI::Button {
                    name: "x"
                    text: "X"
                }
                @GUI::Button {
                    name: "y"
                    text: "Y"
                }
                @GUI::Button {
                    name: "z"
                    text: "Z"
                }
            }
        }

        @GUI::Widget {
            layout: @GUI::HorizontalBoxLayout

            @GUI::TextBox {
                name: "guess_word_text_box"
                placeholder: "Guess Word"
            }
            @GUI::Button {
                name: "guess_word_button"
                text: "Guess"
                fixed_width: 100
            }
        }

        @GUI::Widget {
            layout: @GUI::HorizontalBoxLayout {
                spacing: 10
            }

            @GUI::Button {
                name: "new_game"
                text: "New Game"
                enabled: "false"
            }
            @GUI::Button {
                name: "leave_game"
                text: "Leave Game"
            }
        }
    }
}
