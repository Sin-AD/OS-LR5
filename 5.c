#include <windows.h>
#include <stdio.h>


void main()
{
    int i, x, y;
    HANDLE fhand, hstdin, hstdout;
    DWORD cb, cbw = 0;
    INPUT_RECORD infbuf;
    COORD set1, set2, set3;
    set3.Y = 10;                
 	set3.X=2;
    char buf[255];
    char filename[10]="test.txt";
    char ch;
    hstdout = GetStdHandle( STD_OUTPUT_HANDLE );
    hstdin  = GetStdHandle( STD_INPUT_HANDLE );
    if( hstdin==INVALID_HANDLE_VALUE)return;
    fhand = CreateFileA( filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
    if(fhand==INVALID_HANDLE_VALUE) return;
	//допилить, чтобы файл мог быть бесконечно большим, чтобы выводилось все что есть в файле(цикл), и сколько данных вообще из файла прочитано
    ReadFile ( fhand, buf, 80, &cbw, NULL ); //читаем из файла
    WriteFile( hstdout, buf, cbw, &cbw, NULL ); //выводим на экран
    printf("\n");
    SetConsoleMode(hstdin, ENABLE_EXTENDED_FLAGS|ENABLE_MOUSE_INPUT); // режим ввода в экранный буфер
    infbuf.EventType = MOUSE_EVENT; // нажатие мыши
    while(1)
    {      
        if( ReadConsoleInput(hstdin, &infbuf, 1, &cbw) ) //считываем то на что нажали
        {
            if( infbuf.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED ) ExitProcess(0);    //правая кнопка мыши закрывает программу
            if( infbuf.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)//если левая то продолжаем работу
                {
                    set1.X=infbuf.Event.MouseEvent.dwMousePosition.X; //получаю координату мышки
                    set1.Y=infbuf.Event.MouseEvent.dwMousePosition.Y;
                    ReadConsoleOutputCharacter(hstdout,&ch,1,set1,&cb); //считываем символ, на который нажали
                    if (ch==' ') {FillConsoleOutputAttribute(hstdout,0x00,80,set3,&cbw);}
                if(ch!=' ')  
                {
                   FillConsoleOutputAttribute(hstdout,0x00,80,set3,&cbw);  //установка параметров вывода                       
                    WriteConsoleOutputCharacter(hstdout,buf,80,set3,&cb); // выводим симв0л\о
                    printf(" %c (%d,%d) ",ch,set1.X,set1.Y); //выводим сам символ и его координаты
                    set2=set1;  
                    do 
                    {
                        set1.X--;
                        ReadConsoleOutputCharacter(hstdout,&ch,1,set1,&cb);//считываем циклом слово до края, точнее пробела в одну сторону на уменьшение
                    }
                        while((ch!=' ')&(set1.X>0)); 
                    do 
                    {
                        set2.X++;
                        ReadConsoleOutputCharacter(hstdout,&ch,1,set2,&cb);//считываем циклом слово до края, точнее пробела в другую сторону на увеличение
                    }
                     while((ch!=' ')&&(set2.X<79));                     
                        cbw=set2.X-set1.X+1; //определяем разницу координат
                        ReadConsoleOutputCharacter(hstdout,buf,cbw,set1,&cbw);// всё копируем в буфер
                        FillConsoleOutputAttribute(hstdout,BACKGROUND_RED,cbw,set3,&cbw); //устанавливаем цвет и расположение вывода
                        WriteConsoleOutputCharacter(hstdout,buf,cbw,set3,&cbw);//выводим из буфера
                    }
                }
        }
    }
}         