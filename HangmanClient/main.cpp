#include <LibGUI/Application.h>
#include <LibGUI/Icon.h>
#include <LibGUI/Window.h>
#include <LibMain/Main.h>

#include "HangmanClientWidget.h"

ErrorOr<int> serenity_main(Main::Arguments arguments)
{
    auto app = TRY(GUI::Application::try_create(arguments));

    auto window = TRY(GUI::Window::try_create());
    window->resize(500, 400);
    window->center_on_screen();

    window->set_title("Hangman Client");
    window->set_resizable(false);
    window->set_main_widget<HangmanClientWidget>();

    window->show();

    return app->exec();
}
