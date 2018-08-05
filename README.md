# TrianglePuzzleSolver
```
A program that solves the famous triangle peg game.
Press the right arrow key to see the next best move or press the left arrow key to reverse the previous move.
```
![Alt text](/PegGameScreenshot.png?raw=true)

# How to play:
The goal of the game is to have one peg remaining on the screen. Pegs are removed by having a peg jump over another and land into an empty space; the peg jumped over is removed.
Once the game has started, click on the first peg you want removed; this creates the first hole to jump into.
Click and drag a peg over another and into the hole. This will remove the peg jumped over automatically. If you're confused, just hit the right arrow button to see an example game played out.
The game is built to automatically find the next best move, if there is one. By hitting the right arrow button, the game will make the move that leads to a single peg state -- if there exists such a move.
If you've gotten yourself into a game state that cannot end in a single peg, the right arrow key will do nothing. But no worries, you can (repeatedly) press the left arrow button to undo your last move(s).

# Troubleshooting
Some set up is neccessary if you see a black screen or warning about missing *.dll

The .exe is usually located in a Debug or Release folder in the solution directory (not project directory)
```
         .exe locations commonly in the form of:
         \TrianglePuzzleGame\TrianglePuzzleGame\Debug
         \TrianglePuzzleGame\TrianglePuzzleGame\Release
```

Solution to Black screen:
```
        Make sure you copy the assets folder to the location of your .exe;
        this contains the image files you need at runtime.
        TrianglePuzzleGame\TrianglePuzzleGame\TrianglePuzzleSFML\assets
```
Solution to missing DLLs:
```
        Make sure you copy all of the .dll files.
        The dll files are located at: TrianglePuzzleGame\TrianglePuzzleGame\TrianglePuzzleSFML\sfml\lib
        NOTE: you only need to copy the .dll files(do not copy .ilk, .exp, pdb, or any other files).
        Here are the files that need to be moved to your .exe folder:
                sfml_audio-2.dll
                sfml-audio-d-2.dll
                sfml-graphics-2.dll
                sfml-graphics-2-d.dll
                sfml-network-2.dll
                sfml-network-d-2.dll
                sfml-system-2.dll
                sfml-system-2-d.dll
                sfml-window-2.dll
                sfml-window-d-2.dll
If these steps do not work, the sfml library may need to be re-built for your windows platform.
```
