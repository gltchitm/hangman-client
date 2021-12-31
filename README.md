# Hangman Client
Alternate [Hangman](https://github.com/gltchitm/hangman) client for [SerenityOS](https://github.com/SerenityOS/serenity).

## Installation
Clone the `HangmanClient` folder into `Userland/Applications` and add the following to `Userland/Applications/CMakeLists.txt`:
```
add_subdirectory(HangmanClient)
```
...and rebuild as per usual. You should then be able to execute the program by running `HangmanClient`.

## Known Issues
When playing in remote multiplayer, your remote partner using "Guess Word" fails to make it your turn. This causes both players to be stuck on "Partner's Turn". It looks like this is an issue with SerenityOS's LibWebsocket failing to handle the two messages sent by the game server back-to-back.

## License
[MIT](LICENSE)
