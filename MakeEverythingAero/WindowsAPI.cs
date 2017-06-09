using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Principal;
using System.Text;
using System.Threading.Tasks;
using System.Management;
using Microsoft.Win32;

namespace MakeEverythingAero
{
    static class WindowsAPI
    {
        static WindowsAPI()
        {
            var currentUser = WindowsIdentity.GetCurrent();
            if (currentUser != null && currentUser.User != null)
            {
                UpdateModeFromRegistry();
                var wqlEventQuery = new EventQuery(string.Format(@"SELECT * FROM RegistryValueChangeEvent WHERE Hive='HKEY_USERS' AND KeyPath='{0}\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\ImmersiveShell' AND ValueName='TabletMode'", currentUser.User.Value));
                var managementEventWatcher = new ManagementEventWatcher(wqlEventQuery);
                managementEventWatcher.EventArrived += ManagementEventWatcher_EventArrived;
                managementEventWatcher.Start();
            }
        }

        private static void ManagementEventWatcher_EventArrived(object sender, EventArrivedEventArgs e)
        {
            UpdateModeFromRegistry();
        }

        private static void UpdateModeFromRegistry()
        {
            var tabletMode = (int)Registry.GetValue("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\ImmersiveShell", "TabletMode", 0);
            if (tabletMode == 1)
            {
                IsTabletMode = true;
            }
            else
            {
                IsTabletMode = false;
            }
        }

        public static bool IsTabletMode { get; private set; }

        [DllImport("User32.dll")]
        public static extern bool SetWindowCompositionAttribute(IntPtr hwnd, ref WindowCompositionAttributeData pAttrData);

        [DllImport("User32.dll")]
        public static extern int GetWindowThreadProcessId(IntPtr hWnd, out int lpdwProcessId);

        [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        static extern int GetWindowTextLength(IntPtr hWnd);

        [DllImport("User32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        static extern long GetWindowText(IntPtr hwnd, StringBuilder lpString, long cch);

        public static string GetWindowText(IntPtr hwnd)
        {
            int length = GetWindowTextLength(hwnd);
            if (length == 0) return string.Empty;
            StringBuilder builder = new StringBuilder(length);
            GetWindowText(hwnd, builder, length + 1);
            return builder.ToString();
        }

        [DllImport("USER32.DLL")]
        private static extern bool IsWindowVisible(IntPtr hWnd);

        private delegate bool EnumWindowsProc(IntPtr hWnd, int lParam);

        [DllImport("USER32.DLL")]
        private static extern IntPtr GetShellWindow();

        /// <summary>Returns a dictionary that contains the handle and title of all the open windows.</summary>
        /// <returns>A dictionary that contains the handle and title of all the open windows.</returns>
        public static List<IntPtr> GetOpenedWindows()
        {
            IntPtr shellWindow = GetShellWindow();
            List<IntPtr> windows = new List<IntPtr>();

            EnumWindows((IntPtr hWnd, int lParam) =>
            {
                if (hWnd == shellWindow) return true;
                if (!IsWindowVisible(hWnd)) return true;
                windows.Add(hWnd);
                return true;
            }, 0);
            return windows;
        }

        [DllImport("USER32.DLL")]
        private static extern bool EnumWindows(EnumWindowsProc enumFunc, int lParam);


        [DllImport("User32.dll")]
        public static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

        [DllImport("User32.dll")]
        public static extern IntPtr FindWindowEx(IntPtr hwndParent, IntPtr hwndChildAfter, string lpClassName, string lpWindowName);

        [DllImport("kernel32.dll")]
        public static extern IntPtr LoadLibrary(string path);

        [DllImport("User32.dll")]
        public static extern IntPtr GetProcAddress(IntPtr module, string path);

        [DllImport("User32.dll")]
        public static extern IntPtr SetWindowsHookExA(int lParam, IntPtr hookProc, IntPtr module, int lParam2);

        [DllImport("User32.dll")]
        public static extern bool UnhookWindowsHookEx(IntPtr module);

        [DllImport("User32.dll")]
        public static extern bool GetMessage(ref IntPtr lpMsg, IntPtr hWnd, int wMsgFilterMin, int wMsgFilterMax);
    }
}
