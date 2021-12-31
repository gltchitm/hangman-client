serenity_component(
    HangmanClient
    TARGETS HangmanClient
)

compile_gml(HangmanClientWidget.gml HangmanClientWidgetGML.h hangman_client_gml)

set(SOURCES
    HangmanClientWidgetGML.h
    main.cpp
    HangmanClientWidget.h
    HangmanClientWidget.cpp
)

serenity_app(HangmanClient ICON filetype-executable)
target_link_libraries(HangmanClient LibGUI LibMain LibWebSocket)
