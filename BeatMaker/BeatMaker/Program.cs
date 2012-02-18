using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using SGP;

namespace BeatMaker
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
           // Application.Run(new BeatMaker());

            // Making the form
            BeatMaker theBeatMaker = new BeatMaker();            

            // Displaying it
            theBeatMaker.Show();

            // Making main program loop
            while (theBeatMaker.bRunning)
            {
                theBeatMaker.Update();

                theBeatMaker.Render();

                Application.DoEvents();
            }
            
        }
    }
}
