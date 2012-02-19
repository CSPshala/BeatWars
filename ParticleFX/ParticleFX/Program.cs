using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace ParticleFX
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            FrmMain MainWindow = new FrmMain();
            MainWindow.Show();

            while (MainWindow.InEditor)
            {
                MainWindow.UpdateFunction();
                MainWindow.RenderFunction();
                Application.DoEvents();
            }
        }
    }
}
