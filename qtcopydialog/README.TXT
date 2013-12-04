Copy Dialog v2.2

The Copy Dialog component provides copy functionality with a
progress dialog.

The copy dialog is useful when copying takes a long time. The copy
dialog doesn't block the GUI and provides feedback on the progress
of copy operation.

Known bugs:

The user confirmation dialog may enter a busy-wait loop, eating
CPU. This is a Qt problem, and is fixed in Qt 4.1.

