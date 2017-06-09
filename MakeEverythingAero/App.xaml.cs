using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Diagnostics;
using System.Threading;
using static MakeEverythingAero.WindowsAPI;
using System.IO;

namespace MakeEverythingAero
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        System.Windows.Forms.NotifyIcon notifyIcon = new System.Windows.Forms.NotifyIcon();
        
        private void CreateTray()
        {
            notifyIcon.Icon = MakeEverythingAero.Properties.Resources.Icon;
            notifyIcon.Text = "Aero";
            notifyIcon.ContextMenu = new System.Windows.Forms.ContextMenu(
                new[]{
                    new System.Windows.Forms.MenuItem("Disabled", (s, e) => AeroFXs.Current = AeroFXs.Disabled),
                    new System.Windows.Forms.MenuItem("Blur", (s, e) => AeroFXs.Current = AeroFXs.Blur),
                    new System.Windows.Forms.MenuItem("Transparent", (s, e) => AeroFXs.Current = AeroFXs.Transparent),
                    new System.Windows.Forms.MenuItem("Exit", (s, e) => Shutdown()),
                }
                );
            notifyIcon.Visible = true;
        }

        private void Application_Startup(object sender, StartupEventArgs args)
        {
            CreateTray();
            ApplyFX();
        }

        private void Application_Exit(object sender, ExitEventArgs e)
        {
            notifyIcon.Dispose();
        }

        private static async void ApplyFX()
        {
            await Task.Delay(1);
            do
            {
                AeroFXs.Apply();
                await Task.Delay(20);
            } while (true);
        }
    }
}
