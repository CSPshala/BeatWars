////////////////////////////////////////////////////////////////////////////////////////
//  File			:	"ManagedTextureManager.cs"
//
//  Author			:	David Brown
//                      Ron Powell
//                      Rob Martinez
//                      John OLeske
//
//  Purpose			:	Wrapper class for Managing Managed DirectX Textures.
//
//  NOTE:   In order to use these wrapper classes you will need
//          to add References to the following assemblies to your Solution:
//
//          -Microsoft.DirectX              version: 1.0.2902.0 runtime: v1.1.4322
//          -Microsoft.DirectX.Direct3D     version: 1.0.2902.0 runtime: v1.1.4322
//          -Microsoft.DirectX.Direct3DX    version: 1.0.2902.0 runtime: v1.1.4322
////////////////////////////////////////////////////////////////////////////////////////
using System;
using System.Windows.Forms;         // for MessageBox
using System.Collections.Generic;   // for ArrayList

using System.Diagnostics;           // for Debug.Assert

using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

namespace SGP
{
    public sealed class ManagedTextureManager
    {
        struct TEXTURE
        {
            public string fileName;
            public Microsoft.DirectX.Direct3D.Texture texture;
            public int width;
            public int height;
        }

        System.Collections.ArrayList textures = null;
        Microsoft.DirectX.Direct3D.Device device = null;
        Microsoft.DirectX.Direct3D.Sprite sprite = null;

        // thread-safe singleton
        static readonly ManagedTextureManager instance = new ManagedTextureManager();

        // Explicit static constructor to tell C# compiler
        // not to mark type as beforefieldinit
        static ManagedTextureManager()
        {
        }

        ManagedTextureManager()
        {
        }

        /// <summary>
        /// Gets the instance of the singleton.
        /// </summary>
        public static ManagedTextureManager Instance
        {
            get
            {
                return instance;
            }
        }

        /// <summary>
        /// Initializes the Managed Texture Manager.
        /// </summary>
        /// <param name="device">Direct3D device.</param>
        /// <param name="sprite">Sprite device.</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool InitManagedTextureManager(Microsoft.DirectX.Direct3D.Device device, Microsoft.DirectX.Direct3D.Sprite sprite)
        {
            this.device = device;
            this.sprite = sprite;
            this.textures = new System.Collections.ArrayList();

            return (device != null && sprite != null);
        }

        /// <summary>
        /// Cleans up all of the loaded textures.
        /// </summary>
        public void ShutdownManagedTextureManager()
        {
            for (int i = 0; i < textures.Count; i++)
            {
                // Dispose of the textures
                TEXTURE t = (TEXTURE)textures[i];
                if(t.texture != null)
                    t.texture.Dispose();
                t.fileName = "";
                t.width = -1;
                t.height = -1;

            }

            // Clear the list of all loaded textures.
            textures.Clear();
        }

        /// <summary>
        /// Loads a texture into the texture manager.
        /// </summary>
        /// <param name="fileName">File name of the texture to load.</param>
        /// <param name="colorkey">Transparent color (0 for no color key).  Use Color.FromArgb() then do Color.ToArgb() (Example:  Color.FromArgb(255, 255, 0, 255).ToArgb() for magenta)</param>
        /// <returns>The id for the loaded texture, -1 on failure.</returns>
        public int LoadTexture(string fileName, int colorkey)
        {
            // See if we have loaded the texture before and return its id
            for (int i = 0; i < textures.Count; i++)
            {
                TEXTURE t = (TEXTURE)textures[i];

                if (t.fileName == fileName)
                    return i;
            }

            // we haven't loaded it before so...
            // look for an open spot
            int id = -1;
            for (int i = 0; i < textures.Count; i++)
            {
                TEXTURE t = (TEXTURE)textures[i];

                if (t.texture == null)
                {
                    id = i;
                    break;
                }
            }

            // if we didn't find an open spot, load it into a new one
            if (id == -1)
            {
                // A temp texture object.
                TEXTURE loaded;

                // Copy the filename of the loaded texture.
                loaded.fileName = fileName;
                loaded.texture = null;
                loaded.width = -1;
                loaded.height = -1;

                try
                {
                    loaded.texture = TextureLoader.FromFile(device, fileName, 0, 0, 1, Usage.None, Format.Unknown, Pool.Managed, Filter.None, Filter.None, colorkey);
                }
                catch (Exception)
                {
                    MessageBox.Show("Failed to load texture: " + fileName, "ManagedTextureManager::Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return -1;
                }

                // Get Width/Height of the texture.
                loaded.width    = loaded.texture.GetSurfaceLevel(0).Description.Width;
                loaded.height   = loaded.texture.GetSurfaceLevel(0).Description.Height;

                // Put the texture into the list.
                textures.Add(loaded);

                // Return the id of the texture.
                return textures.Count - 1;
            }
            // we found an open spot
            else
            {
                TEXTURE t = (TEXTURE)textures[id];

                // Make sure the texture has been released.
                if (t.texture != null)
                {
                    t.texture.Dispose();
                    t.texture = null;
                }

                // Copy the filename of the loaded texture.
                t.fileName = fileName;

                // Load the texture from the given file.
                try
                {
                    t.texture = TextureLoader.FromFile(device, fileName, 0, 0, 1, Usage.None, Format.Unknown, Pool.Managed, Filter.None, Filter.None, colorkey);
                }
                catch (Exception)
                {
                    MessageBox.Show("Failed to load texture: " + fileName, "ManagedTextureManager::Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return -1;
                }

                // Get Width/Height of the texture.
                t.width     = t.texture.GetSurfaceLevel(0).Description.Width;
                t.height    = t.texture.GetSurfaceLevel(0).Description.Height;

                textures[id] = t;

                if (!textures[id].Equals(t))
                    MessageBox.Show("theys not equal!!");

                // Return the id of the texture.
                return id;
            }
        }

        /// <summary>
        /// Releases the memory of a texture.
        /// </summary>
        /// <param name="id">Id of the texture to release.</param>
        public void ReleaseTexture(int id)
        {
            // Make sure the id is in range.
            Debug.Assert((id > -1 && id < textures.Count), "id is out of range");

            // Do a lazy delete and leave this spot empty.
            TEXTURE t = (TEXTURE)textures[id];

            t.texture.Dispose();
            t.texture = null;

            t.fileName = "";
            t.width = -1;
            t.height = -1;

            // Put it back in textures array changed.
            textures[id] = t;

            if (!textures[id].Equals(t))
                MessageBox.Show("theys not equal!!");
        }

        /// <summary>
        /// Gets the width of a specific texture.
        /// </summary>
        /// <param name="id">Id of the texture to get the width of.</param>
        /// <returns>The width of the specific texture.</returns>
        public int GetTextureWidth(int id)
        {
            // Make sure the id is in range.
            Debug.Assert((id > -1 && id < textures.Count), "id is out of range");

            return ((TEXTURE)textures[id]).width;
        }

        /// <summary>
        /// Gets the height of a specific texture.
        /// </summary>
        /// <param name="id">Id of the texture to get the height of.</param>
        /// <returns>The height of the specific texture.</returns>
        public int GetTextureHeight(int id)
        {
            // Make sure the id is in range.
            Debug.Assert((id > -1 && id < textures.Count), "id is out of range");

            return ((TEXTURE)textures[id]).height;
        }

        /// <summary>
        /// Draws a texture to the screen.
        /// </summary>
        /// <param name="id">Id of the texture to draw.</param>
        /// <param name="x">X position to draw at.</param>
        /// <param name="y">Y position to draw at.</param>
        /// <param name="scalex">X scale.</param>
        /// <param name="scaley">Y scale.</param>
        /// <param name="section">Section of the image to draw (Use Rectangle.Empty for the whole surface).</param>
        /// <param name="rotx">X position on the texture to rotate around.</param>
        /// <param name="roty">Y position on the texture to rotate around.</param>
        /// <param name="rot">Amount to rotate by (in radians).</param>
        /// <param name="color">Color to modulate the image with (use Color.ToArgb()).</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool Draw(int id, int x, int y,
                         float scalex, float scaley, System.Drawing.Rectangle section,
                         int rotx, int roty, float rot, int color)
        {
            // Make sure the id is in range.
            Debug.Assert((id > -1 && id < textures.Count), "id is out of range");

            // Make sure the sprite was created.
            if (sprite == null)
                return false;

            // Initialize to Identity.
            Matrix result = Matrix.Identity;

            // Scale the sprite.
            Matrix scale = Matrix.Scaling(scalex, scaley, 1.0f);
            result *= scale;

            // Rotate the sprite.
            Matrix translate = Matrix.Translation(-rotx, -roty, 0.0f);
            result *= translate;

            Matrix rotate = Matrix.RotationZ(rot);
            result *= rotate;

            translate = Matrix.Translation(rotx, roty, 0.0f);
            result *= translate;

            // Translate the sprite.
            translate = Matrix.Translation(x, y, 0.0f);
            result *= translate;

            // Apply the transform.
            sprite.Transform = result;

            // Draw the sprite.
            if(color!=0)
                sprite.Draw( ((TEXTURE)textures[id]).texture, section, Vector3.Empty, Vector3.Empty, System.Drawing.Color.FromArgb(color) );
            else
                sprite.Draw( ((TEXTURE)textures[id]).texture, section, Vector3.Empty, Vector3.Empty, System.Drawing.Color.White);
            

            // Move the world back to identity.
            sprite.Transform = Matrix.Identity;

            // success.
            return true;
        }
    } // end ManagedTextureManager
}
