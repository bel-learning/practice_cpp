#include<ncurses.h>
#include<string>
#include "../util.h"
#include "../calendar.h"
using namespace std;


void drawButton(int y, int x, const std::string& label, bool selected, WINDOW * win) {
    if (selected) {
        wattron(win, A_REVERSE);   // Highlight the button if selected
    }
  
    mvwprintw(win,y, x, "[%s]", label.c_str());
    wattroff(win,A_REVERSE);  // Turn off highlighting
}

int GetMenuView(WINDOW * main, Calendar * cal) {
    int rows, cols;
    getmaxyx(main, rows, cols);
    int sRows = rows/2, sCols = cols/2;

    WINDOW * subWindow = subwin(main, sRows, sCols, rows/4, cols/4);
    box(subWindow, 0,0);
    curs_set(0);

    std::string DailyCal = "Daily Calendar";
    std::string WeeklyCal = "Weekly Calendar";
    std::string MonthlyCal = "Monthly Calendar";
    std::string addEvent = "Add Event";

    std::string Quit = "Quit";
    
    int s = 3;
    int debugPos = 1;
    int selectedButton = 0;

    while(true) {
        
        mvwprintw(subWindow,1,sCols/2 - 5, "Main Menu");
        wattron(subWindow, A_BOLD);

        mvwprintw(subWindow,s,4, "Calendar Choices");
        wattroff(subWindow, A_BOLD);
        
        drawButton(s+1, 7, MonthlyCal, selectedButton == 0, subWindow);
        drawButton(s+2, 7, WeeklyCal, selectedButton == 1, subWindow);
        drawButton(s+3, 7, DailyCal, selectedButton == 2, subWindow);
        drawButton(s+4, 4, addEvent, selectedButton == 3, subWindow);
        drawButton(s+5, 4, Quit, selectedButton == 4, subWindow);
    
        wrefresh(subWindow);
        
        int ch = getch();

        switch (ch) {
            case KEY_UP:
            case 2965:
                selectedButton--;
                if (selectedButton < 0) {
                    selectedButton = 4;
                }
                break;
            case KEY_DOWN:
            case 2966:
                selectedButton++;
                if (selectedButton > 4) {
                    selectedButton = 0;
                }
                break;

            
            case KEY_ENTER:
            case 10:  // Enter key
                wrefresh(subWindow);
                wclear(subWindow);
                delwin(subWindow);
                
                return selectedButton;
                break;
            case 'q':
                return 0;
                break;
        }

    }

};