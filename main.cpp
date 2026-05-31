#include <iostream>
#include "display/DisplayEngine.h"
#include "terminal_display/TerminalDisplay.h"
using namespace std;


int main() {
    DisplayEngine display;
    display.Run();
    return 0;
}

// int main() {
//     TerminalDisplay display;
//     display.Run();
//     return 0;
// }