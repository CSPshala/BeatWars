// System References
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Bitmap = System.Drawing.Bitmap;
using Image = System.Drawing.Image;
using Size = System.Drawing.Size;

// XNA References
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using System.Windows.Forms;
using System.IO;

/***************************************************************
 * TextureUtility.cs
 ***************************************************************
 *  Author:         Jeffery Sventora
 *  Created:        2/7/2012
 *  Last Edit:      2/7/2012
 ***************************************************************
 *  Description:    
 ***************************************************************
 * This file will give helper functions to handle Textures
 * in a Windows Form Environment.
 ***************************************************************/

namespace ParticleFX
{
    public static class TextureUtility
    {
        public static Texture2D TextureFromFile(string FileName, GraphicsDevice Device)
        {
            // We first need to load an image from the Hard Drive
            Image LoadedImage;

            // Try to load the image based on a file name
            try
            {
                LoadedImage = Image.FromFile(FileName);
            }
            catch
            {
                MessageBox.Show("Failed to load " + FileName + "! Please make sure it is a valid image file!", "Image Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return null;
            }

            // Create a stream where we can save that image into memory as a PNG
            MemoryStream MStream = new MemoryStream();
            LoadedImage.Save(MStream, System.Drawing.Imaging.ImageFormat.Png);

            // The Texture to return
            Texture2D FromFile;

            // Try to pull the image out of the stream and convert it to a Texture2D
            try
            {
                FromFile = Texture2D.FromStream(Device, MStream);
            }
            catch
            {
                MessageBox.Show("Failed to convert " + FileName + " into a Texture2D!", "Texture Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return null;
            }

            // Return the newly loaded Texture2D!
            return FromFile;
        }
        public static Texture2D TextureFromImage(Image Source, GraphicsDevice Device)
        {
            // Create a stream where we can save that image into memory as a PNG
            MemoryStream MStream = new MemoryStream();
            Source.Save(MStream, System.Drawing.Imaging.ImageFormat.Png);

            // The Texture to return
            Texture2D FromFile;

            // Try to pull the image out of the stream and convert it to a Texture2D
            try
            {
                FromFile = Texture2D.FromStream(Device, MStream);
            }
            catch
            {
                MessageBox.Show("Failed to convert Image to Texture2D!", "Texture Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return null;
            }

            // Return the newly loaded Texture2D!
            return FromFile;
        }
        public static Image ImageFromTexture(Texture2D Source)
        {
            Image ToReturn;
            MemoryStream MStream = new MemoryStream();
            Source.SaveAsPng(MStream, Source.Width, Source.Height);
            ToReturn = Image.FromStream(MStream);

            MStream.Dispose();
            MStream = null;
            GC.Collect();

            return ToReturn;
        }
    }
}
