using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Graphics;
using System.IO;

/***************************************************************
 * ParticleTexture.cs
 ***************************************************************
 *  Author:         Jeffery Sventora
 *  Created:        2/10/2012
 *  Last Edit:      2/14/2012
 ***************************************************************
 *  Description:    
 ***************************************************************
 * This object will contain a Texture2D and a string for the
 * name. It's only use is for diversion within a list.
 ***************************************************************/

namespace ParticleFX
{
    public class ParticleTexture
    {
        // Member Fields
        Texture2D texture;
        string name;

        // Member Properties
        public Texture2D Texture
        {
            get { return texture; }
            protected set { texture = value; }
        }
        public string Name
        {
            get { return name; }
            protected set { name = value; }
        }
        public int Width
        {
            get
            {
                if (Texture != null)
                    return Texture.Width;
                else
                    return 0;
            }
        }
        public int Height
        {
            get
            {
                if (Texture != null)
                    return Texture.Height;
                else
                    return 0;
            }
        }

        // Constructors
        public ParticleTexture()
        {
            Name = "";
        }
        public ParticleTexture(string File, GraphicsDevice Device)
        {
            Load(File, Device);
        }

        // Methods
        public void Load(string File, GraphicsDevice Device)
        {
            Name = Path.GetFileName(File);
            Texture = TextureUtility.TextureFromFile(File, Device);
        }
        public override string ToString()
        {
            return Name + " - W: " + Width.ToString() + " H: " + Height.ToString();
        }
    }
}
