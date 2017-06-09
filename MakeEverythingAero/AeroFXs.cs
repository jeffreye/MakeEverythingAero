using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace MakeEverythingAero
{
    class AeroFXs
    {
        public static WindowCompositionAttributeData Current { get; set; } = Blur;

        public static WindowCompositionAttributeData Disabled
        {
            get
            {
                AccentPolicy accentPolicy = default(AccentPolicy);
                accentPolicy.AccentState = AccentState.ACCENT_DISABLED;
                accentPolicy.AccentFlags = 2;
                accentPolicy.AnimationId = 0;
                accentPolicy.GradientColor = 0;
                int num = Marshal.SizeOf(accentPolicy);
                IntPtr intPtr = Marshal.AllocHGlobal(num);
                Marshal.StructureToPtr(accentPolicy, intPtr, false);
                return new WindowCompositionAttributeData
                {
                    Attribute = WindowCompositionAttribute.WCA_ACCENT_POLICY,
                    SizeOfData = num,
                    Data = intPtr
                };
            }
        }

        public static WindowCompositionAttributeData Transparent
        {
            get
            {
                AccentPolicy accentPolicy = default(AccentPolicy);
                accentPolicy.AccentState = AccentState.ACCENT_ENABLE_TRANSPARENTGRADIENT;
                accentPolicy.AccentFlags = 2;
                accentPolicy.AnimationId = 0;
                accentPolicy.GradientColor = 0;
                int num = Marshal.SizeOf(accentPolicy);
                IntPtr intPtr = Marshal.AllocHGlobal(num);
                Marshal.StructureToPtr(accentPolicy, intPtr, false);
                return new WindowCompositionAttributeData
                {
                    Attribute = WindowCompositionAttribute.WCA_ACCENT_POLICY,
                    SizeOfData = num,
                    Data = intPtr
                };
            }
        }

        public static WindowCompositionAttributeData Blur
        {
            get
            {
                AccentPolicy accentPolicy = default(AccentPolicy);
                accentPolicy.AccentState = AccentState.ACCENT_ENABLE_BLURBEHIND;
                accentPolicy.AccentFlags = 0;
                accentPolicy.AnimationId = 0;
                accentPolicy.GradientColor = 0;
                int num = Marshal.SizeOf(accentPolicy);
                IntPtr intPtr = Marshal.AllocHGlobal(num);
                Marshal.StructureToPtr(accentPolicy, intPtr, false);
                return new WindowCompositionAttributeData
                {
                    Attribute = WindowCompositionAttribute.WCA_ACCENT_POLICY,
                    SizeOfData = num,
                    Data = intPtr
                };
            }
        }


        static bool SetAll(string lpClassName, ref WindowCompositionAttributeData efffect)
        {
            bool any = false;
            IntPtr trayPtr = new IntPtr(0);
            do
            {
                trayPtr = WindowsAPI.FindWindowEx(IntPtr.Zero, trayPtr, lpClassName, null);
                if ((int)trayPtr > 0)
                    WindowsAPI.SetWindowCompositionAttribute(trayPtr, ref efffect);
            }
            while ((int)trayPtr > 0);

            return any;
        }

        static bool SetAll(string lpClassName, string process, ref WindowCompositionAttributeData efffect)
        {
            bool any = false;
            IntPtr trayPtr = new IntPtr(0);
            var p = System.Diagnostics.Process.GetProcessesByName(process).FirstOrDefault();
            do
            {
                trayPtr = WindowsAPI.FindWindowEx(IntPtr.Zero, trayPtr, lpClassName, null);
                if ((int)trayPtr > 0)
                {
                    WindowsAPI.GetWindowThreadProcessId(trayPtr, out int pid);
                    if (p.Id == pid)
                        WindowsAPI.SetWindowCompositionAttribute(trayPtr, ref efffect);
                }
            }
            while ((int)trayPtr > 0);

            return any;
        }

        static bool Set(string lpClassName, string lpWindowName, ref WindowCompositionAttributeData efffect)
        {
            IntPtr wndPtr = WindowsAPI.FindWindow(lpClassName, lpWindowName);
            if ((int)wndPtr > 0)
            {
                WindowsAPI.SetWindowCompositionAttribute(wndPtr, ref efffect);
                return true;
            }
            return false;
        }


        public static void Apply()
        {
            var aero = Current;
            if (!WindowsAPI.IsTabletMode)
            {
                // Taskbar
                if (Set("Shell_TrayWnd", null, ref aero))
                {
                    SetAll("Shell_SecondaryTrayWnd", ref aero);
                }
                Set("NotifyIconOverflowWindow", null, ref aero);

                // Start/Action Center/Network/Date and Time
                SetAll("Windows.UI.Core.CoreWindow", "ShellExperienceHost", ref aero);

            }
        }
    }
}
