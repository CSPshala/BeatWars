using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

/***************************************************************
 * BufferedPanel.cs
 ***************************************************************
 *  Author:         Jeffery Sventora
 *  Created:        2/7/2012
 *  Last Edit:      2/7/2012
 ***************************************************************
 *  Description:    
 ***************************************************************
 * This file is a control that inherits Panel, the only 
 * difference is that this panel features double buffering.
 ***************************************************************/

namespace ParticleFX
{
    public class BufferedPanel : Panel
    {
        // Constructor
        public BufferedPanel()
        {
            DoubleBuffered = true;
        }
    }
}
