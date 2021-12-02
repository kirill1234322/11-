// lab11.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "lab11.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <synchapi.h>
#include <iostream>
#include <Tlhelp32.h>
#include <strsafe.h>
#include <string.h>
#include <string>
#include "commdlg.h"
#include "CommCtrl.h"
#include "commdlg.h"


#define MAX_LOADSTRING 100
#define ID_LIST 1
#define ID_LIST2 4
#define ID_BUTTON_1 114
#define ID_BUTTON_2 115
#define IDC_EDIT1 2001


// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

using namespace std;
HWND hList, hList2, hList3;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB11, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB11));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB11));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB11);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (NULL != hWnd)
   {
       CreateWindowEx(0, L"static", L"Напишите ID процесса, который хотите завершить", WS_CHILD | WS_VISIBLE | LBS_WANTKEYBOARDINPUT,
           500, 10, 400, 30, hWnd, NULL, hInst, NULL);


        CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""), ES_AUTOHSCROLL | ES_LEFT | WS_CHILD | WS_VISIBLE | LBS_WANTKEYBOARDINPUT,
           500, 60, 200, 30, hWnd, (HMENU)IDC_EDIT1, hInst, NULL);

        CreateWindowEx(0, L"button", L"Завершить процесс", WS_CHILD | WS_VISIBLE | LBS_WANTKEYBOARDINPUT,
           500, 130, 150, 30, hWnd, (HMENU)ID_BUTTON_1, hInst, NULL);

        CreateWindowEx(0, L"button", L"Создать процессы", WS_CHILD | WS_VISIBLE | LBS_WANTKEYBOARDINPUT,
           500, 320, 500, 30, hWnd, (HMENU)ID_BUTTON_2, hInst, NULL);

        




   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}



VOID PrintModuleList(HANDLE CONST hStdOut, DWORD CONST dwProcessId) {
    MODULEENTRY32 meModuleEntry;
    TCHAR szBuff[1024];
    HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPMODULE, dwProcessId);
    if (INVALID_HANDLE_VALUE == hSnapshot)
    {
        return;
    }

    meModuleEntry.dwSize = sizeof(MODULEENTRY32);
    Module32First(hSnapshot, &meModuleEntry);

    do {
        wsprintf(szBuff, L"%p, %08X, %s\r\n",
            meModuleEntry.modBaseAddr, meModuleEntry.modBaseSize,
            meModuleEntry.szModule);


        SendMessage(hList3, LB_ADDSTRING, 0, (LPARAM)(LPSTR)(szBuff));


    } while (Module32Next(hSnapshot, &meModuleEntry));





}


VOID PrintProcessList(HANDLE CONST hStdOut, int number) 
{
    PROCESSENTRY32 peProcessEntry;
    TCHAR szBuff[1024];
    HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot) {
        return;
    }

    peProcessEntry.dwSize = sizeof(PROCESSENTRY32);
    Process32First(hSnapshot, &peProcessEntry);

    int i = 0;

    if (number == -1) {

        do {



            wsprintf(szBuff, L"%s  PID: %u  \r\n",
                peProcessEntry.szExeFile, peProcessEntry.th32ProcessID);
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)(szBuff));


        } while (Process32Next(hSnapshot, &peProcessEntry));
    }
    else {
        do {


            if (i == number) {
                PrintModuleList(hStdOut, peProcessEntry.th32ProcessID);
                break;
            }
            else i++;


        } while (Process32Next(hSnapshot, &peProcessEntry));
    }
    CloseHandle(hSnapshot);


}






BOOL StartGroupProcessesAsJob(HANDLE hJob, LPCTSTR lpszCmdLine[],
    DWORD nCount, BOOL bInheritHandles, DWORD dwCreationFlags)
{
    // определяем, включен ли вызывающий процесс в задачу
    BOOL bInJob = FALSE;
    IsProcessInJob(GetCurrentProcess(), NULL, &bInJob);
    if (FALSE != bInJob) 
      
    {
        JOBOBJECT_BASIC_LIMIT_INFORMATION jobli = { 0 };
        QueryInformationJobObject(NULL, JobObjectBasicLimitInformation, &jobli, sizeof(jobli), NULL);
        DWORD dwLimitMask = JOB_OBJECT_LIMIT_BREAKAWAY_OK | JOB_OBJECT_LIMIT_SILENT_BREAKAWAY_OK;

        if ((jobli.LimitFlags & dwLimitMask) == 0)
        {
          
            return FALSE;
        }
    }

    // порождаем процессы
    TCHAR szCmdLine[MAX_PATH];
    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;

    for (DWORD i = 0; i < nCount; ++i)
    {
        StringCchCopy(szCmdLine, MAX_PATH, lpszCmdLine[i]);

       
        BOOL bRet = CreateProcess(NULL, szCmdLine, NULL, NULL, bInheritHandles,
            dwCreationFlags | CREATE_SUSPENDED | CREATE_BREAKAWAY_FROM_JOB, NULL, NULL, &si, &pi);


        if (FALSE != bRet)
        {
            // добавляем новый процесс в задание
            AssignProcessToJobObject(hJob, pi.hProcess);
            // возобновляем работу потока нового процесса
            ResumeThread(pi.hThread);
            // закрывает дескриптор потока нового процесса
            CloseHandle(pi.hThread);
            // закрывает дескриптор нового процесса
            CloseHandle(pi.hProcess);

        }
    }
    return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    HWND label, label3;
    int wmId = LOWORD(wParam);
    int wmEvent = HIWORD(wParam);


    switch (message)
    {

    case WM_CREATE:
    {

        

        label = CreateWindowEx(0, L"static", L"\tСписок модулей процесса", WS_CHILD | WS_VISIBLE | LBS_WANTKEYBOARDINPUT,
            30, 270, 400, 30, hWnd, NULL, hInst, NULL);


        label3 = CreateWindowEx(0, L"static", L"\tСписок процессов", WS_CHILD | WS_VISIBLE | LBS_WANTKEYBOARDINPUT,
            30, 10, 400, 30, hWnd, NULL, hInst, NULL);



        hList = CreateWindowEx(0, L"listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_WANTKEYBOARDINPUT | WS_HSCROLL | WS_VSCROLL | LBS_NOTIFY,
            30, 50, 400, 200, hWnd, (HMENU)ID_LIST, hInst, NULL);



        

        SendMessage(hList, WM_SETREDRAW, FALSE, 0L);
        HANDLE CONST hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        PrintProcessList(hStdOut, -1);



        
        SendMessage(hList, WM_SETREDRAW, TRUE, 0L);

        
        InvalidateRect(hList, NULL, TRUE);



       
         
        hList3 = CreateWindowEx(0, L"listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_WANTKEYBOARDINPUT | WS_HSCROLL | WS_VSCROLL | LBS_NOTIFY,
            30, 310, 400, 200, hWnd, (HMENU)ID_LIST2, hInst, NULL);

        PrintProcessList(hStdOut, -1);

        SendMessage(hList3, WM_SETREDRAW, FALSE, 0L);

        SendMessage(hList3, WM_SETREDRAW, TRUE, 0L);

        InvalidateRect(hList3, NULL, TRUE);


        return 0;

    }

    case WM_COMMAND:
        {
        if ((LOWORD(wParam) == ID_BUTTON_1) && (HIWORD(wParam) == BN_CLICKED))
        {
            DWORD PID = GetDlgItemInt(hWnd, IDC_EDIT1, NULL, FALSE);


            DWORD GetProcessByExeName(char* ExeName);

            if (!PID)  MessageBox(hWnd, L"Процесс не найден", L"Caption", MB_OK);
            else
            {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PID);

                if (TerminateProcess(hProcess, 0)) {
                    MessageBox(hWnd, L"Процесс завершен", L"Caption", MB_OK);
                    SendMessage(hList, LB_RESETCONTENT, 0, 0);
                    PrintProcessList(GetStdHandle(STD_OUTPUT_HANDLE), -1);
                }

                else  MessageBox(hWnd, L"Процесс не может быть завершен", L"Caption", MB_OK);
                CloseHandle(hProcess);
            }

        }

        if ((LOWORD(wParam) == ID_BUTTON_2) && (HIWORD(wParam) == BN_CLICKED))
        {
            HANDLE hJob = CreateJobObject(NULL, TEXT(" ")); 
            if (NULL != hJob)
            {
                 
                JOBOBJECT_EXTENDED_LIMIT_INFORMATION jobli = { 0 };

                BOOL bRet = SetInformationJobObject(hJob,
                    JobObjectExtendedLimitInformation, &jobli, sizeof(jobli));

                if (FALSE != bRet)
                {
                    //создание процессов
                    LPCTSTR szCmdLine[] = { TEXT("explorer.exe"), TEXT("cmd.exe"), TEXT("calc.exe")};

                    bRet = StartGroupProcessesAsJob(hJob, szCmdLine,
                        _countof(szCmdLine), FALSE, 0);
                }
                CloseHandle(hJob);
            }
        }       


           if ((LOWORD(wParam) == ID_LIST) && (HIWORD(wParam) == LBN_DBLCLK))
           {

            SendMessage(hList3, LB_RESETCONTENT, 0, 0);

            HANDLE CONST hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            int number = SendMessage(hList, LB_GETCURSEL, 0, wParam); 

            PrintProcessList(hStdOut, number);


           }
           
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
