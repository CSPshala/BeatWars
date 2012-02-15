////////////////////////////////////////////////////////////////////////////////////////
//  File			:	"ManagedDirect3D.cs"
//
//  Authors			:	David Brown
//                      Ron Powel
//                      Rob Martinez
//                      John OLeske
//
//  Purpose			:	Wrapper class for Managed Direct3D.
//
//  NOTE:   In order to use these wrapper classes you will need
//          to add References to the following assemblies to your Solution:
//
//          -Microsoft.DirectX              version: 1.0.2902.0 runtime: v1.1.4322
//          -Microsoft.DirectX.Direct3D     version: 1.0.2902.0 runtime: v1.1.4322
//          -Microsoft.DirectX.Direct3DX    version: 1.0.2902.0 runtime: v1.1.4322
////////////////////////////////////////////////////////////////////////////////////////
using System;               // for Int32
using System.Windows.Forms; // for MessageBox
using System.Collections.Generic; // for List

using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

namespace SGP
{
    public sealed class ManagedDirect3D
    {
        Microsoft.DirectX.Direct3D.Device device = null;
        Microsoft.DirectX.Direct3D.Sprite sprite = null;
        Microsoft.DirectX.Direct3D.Line line = null;
        Microsoft.DirectX.Direct3D.PresentParameters presentParams = null;

        Microsoft.DirectX.Direct3D.Font font = null;
        Microsoft.DirectX.Direct3D.FontDescription fontDescription;

        List<ManagedSwapChain> lSwapChains = new List<ManagedSwapChain>();

        public Microsoft.DirectX.Direct3D.Texture rectTexture = null;

        int ActiveChain = 0;

        // thread-safe singleton
        static readonly ManagedDirect3D instance = new ManagedDirect3D();

        // Explicit static constructor to tell C# compiler
        // not to mark type as beforefieldinit
        static ManagedDirect3D()
        {
        }

        ManagedDirect3D()
        {
        }

        /// <summary>
        /// Gets the instance of the singleton.
        /// </summary>
        public static ManagedDirect3D Instance
        {
            get
            {
                return instance;
            }
        }

        /// <summary>
        /// Gets the Direct3D device.
        /// </summary>
        public Microsoft.DirectX.Direct3D.Device Device
        {
            get
            {
                return device;
            }
        }

        /// <summary>
        /// Gets the Sprite object.
        /// </summary>
        public Microsoft.DirectX.Direct3D.Sprite Sprite
        {
            get
            {
                return sprite;
            }
        }

        /// <summary>
        /// Initializes the Managed Direct3D Wrapper.
        /// 
        /// This overload is designed for displaying a diferent resolution then the controls size.
        /// </summary>
        /// <param name="renderWindow">Window to render to.</param>
        /// <param name="screenWidth">Width of the screen in pixels.</param>
        /// <param name="screenHeight">Height of the screen in pixels.</param>
        /// <param name="vsync">Should the renderer wait for a vsync before drawing?</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool InitManagedDirect3D(System.Windows.Forms.Control renderWindow, int screenWidth, int screenHeight, bool vsync)
        {
            try
            {
                // Now  setup our D3D present parameters
                presentParams = new PresentParameters();
                presentParams.BackBufferWidth = screenWidth;
                presentParams.BackBufferHeight = screenHeight;
                //taking out the option for fullscreen mode
                //presentParams.BackBufferFormat = (isWindowed) ? Format.Unknown : Format.R5G6B5;
                presentParams.BackBufferFormat = Format.Unknown;
                presentParams.BackBufferCount = 1;
                presentParams.MultiSample = MultiSampleType.None;
                presentParams.MultiSampleQuality = 0;
                presentParams.SwapEffect = SwapEffect.Copy;// Discard;
                presentParams.DeviceWindow = renderWindow;
                //taking out the option for fullscreen mode
                //presentParams.Windowed = isWindowed;
                presentParams.Windowed = true;
                presentParams.EnableAutoDepthStencil = false;
                presentParams.FullScreenRefreshRateInHz = 0;
                presentParams.PresentationInterval = (vsync) ? PresentInterval.Default : PresentInterval.Immediate;

                // only Create if we haven't done so already
                if(device == null)
                device = new Device(0, DeviceType.Hardware, renderWindow,
                        CreateFlags.HardwareVertexProcessing, presentParams);

                //Create the SwapChain
            lSwapChains.Add(new ManagedSwapChain(device, presentParams));
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to Create D3D Device", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                return false;
            }

            try
            {
                sprite = new Microsoft.DirectX.Direct3D.Sprite(device);
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to Create the Sprite object", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                return false;
            }

            try
            {
                line = new Microsoft.DirectX.Direct3D.Line(device);

                line.Antialias = true;
                line.Width = 3.0f;
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to Create the Line Object", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                return false;
            }

            try
            {
                fontDescription.FaceName = "arial";
                fontDescription.Quality = FontQuality.Default;
                fontDescription.Weight = FontWeight.Bold;

                font = new Microsoft.DirectX.Direct3D.Font(device, fontDescription);

                line.Antialias = true;
                line.Width = 3.0f;
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to Create the font Object", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                return false;
            }

            return true;
        }

        /// <summary>
        /// find the swap chain in the list and sets that as the active on..
        /// </summary>
        /// <param name="renderWindow">Window to render to.</param>
        /// <returns>true if successful, false otherwise.</returns>
        private bool SetActiveSwapChain(System.Windows.Forms.Control renderWindow)
        {
            // Find the swap chain
            ActiveChain = -1;
            for (int i = 0; i < lSwapChains.Count; i++)
            {
                if (lSwapChains[i].Chain.Disposed)
                {
                    lSwapChains[i].Reset(device);
                    //return true;
                }
                if (lSwapChains[i].Chain.PresentParameters.DeviceWindow == renderWindow)
                {
                    ActiveChain = i;
                    break;
                }
            }
            if (ActiveChain < 0)
            {
                DialogResult r = MessageBox.Show("Not a valid render window", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }
            return true;
        }

        /// <summary>
        /// Initializes the Managed Direct3D Wrapper.
        /// </summary>
        /// <param name="renderWindow">Window to render to.</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool InitManagedDirect3D(System.Windows.Forms.Control renderWindow)
        {
            return InitManagedDirect3D(renderWindow, renderWindow.Width, renderWindow.Height, false);
        }

        /// <summary>
        /// Initializes the Managed Direct3D Wrapper.
        /// </summary>
        /// <param name="renderWindow">Window to render to.</param>
        /// <param name="vsync">Should the renderer wait for a vsync before drawing?</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool InitManagedDirect3D(System.Windows.Forms.Control renderWindow, bool vsync)
        {
            return InitManagedDirect3D(renderWindow, renderWindow.Width, renderWindow.Height, vsync);
        }

        /// <summary>
        /// Changes the display parameters of the Managed Direct3D Wrapper.
        /// 
        /// Use this to change what window you render to.
        /// This overload is designed for displaying a diferent resolution size then the controls size
        /// </summary>
        /// <param name="renderWindow">Window to render to.</param>
        /// <param name="screenWidth">Width of the screen in pixels.</param>
        /// <param name="screenHeight">Height of the screen in pixels.</param>
        /// <param name="vsync">Should the renderer wait for a vsync before drawing?</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool ChangeDisplayParam(System.Windows.Forms.Control renderWindow, int screenWidth, int screenHeight, bool vsync)
        {
            try
            {
                // Now  setup our D3D present parameters
                presentParams = new PresentParameters();
                presentParams.BackBufferWidth = screenWidth;
                presentParams.BackBufferHeight = screenHeight;
                // taking out option for fullscreen mode
                //presentParams.BackBufferFormat = (isWindowed) ? Format.Unknown : Format.R5G6B5;
                presentParams.BackBufferFormat = Format.Unknown;
                presentParams.BackBufferCount = 1;
                presentParams.MultiSample = MultiSampleType.None;
                presentParams.MultiSampleQuality = 0;
                presentParams.SwapEffect = SwapEffect.Copy;// Discard;
                presentParams.DeviceWindow = renderWindow;
                // taking out option for fullscreen mode
                //presentParams.Windowed = isWindowed;
                presentParams.Windowed = true;
                presentParams.EnableAutoDepthStencil = false;
                presentParams.FullScreenRefreshRateInHz = 0;
                presentParams.PresentationInterval = (vsync) ? PresentInterval.Default : PresentInterval.Immediate;

                // Find the swap chain
                SetActiveSwapChain(renderWindow);
                
                font.OnLostDevice();
                line.OnLostDevice();
                sprite.OnLostDevice();
                device.Reset(presentParams);
                sprite.OnResetDevice();
                line.OnResetDevice();
                font.OnResetDevice();

                lSwapChains[ActiveChain].Reset(device, presentParams);
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to change the render window", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                return false;
            }

            return true;
        }

        /// <summary>
        /// Changes the display parameters of the Managed Direct3D Wrapper.
        /// 
        /// Use this to change what window you render to.
        /// </summary>
        /// <param name="renderWindow">Window to render to.</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool ChangeDisplayParam(System.Windows.Forms.Control renderWindow)
        {
            return ChangeDisplayParam(renderWindow, renderWindow.Width, renderWindow.Height, false);
        }

        /// <summary>
        /// Changes the display parameters of the Managed Direct3D Wrapper.
        /// 
        /// Use this to change what window you render to.
        /// </summary>
        /// <param name="renderWindow">Window to render to.</param>
        /// <param name="vsync">Should the renderer wait for a vsync before drawing?</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool ChangeDisplayParam(System.Windows.Forms.Control renderWindow, bool vsync)
        {
            return ChangeDisplayParam(renderWindow, renderWindow.Width, renderWindow.Height, vsync);
        }

        /// <summary>
        /// Handles resetting the device (Private utility function).
        /// </summary>
        private void Reset()
        {
            font.OnLostDevice();
            line.OnLostDevice();
            sprite.OnLostDevice();

            device.Reset(presentParams);

            font.OnResetDevice();
            sprite.OnResetDevice();
            line.OnResetDevice();

            for (int i = 0; i < lSwapChains.Count; i++ )
            {
                lSwapChains[i].Reset(device);
            }
        }
        
        /// <summary>
        /// Clears the screen to a specific color.
        /// </summary>
        /// <param name="renderWindow">The window we want to clear.</param>
        /// <param name="color">What color to clear the background to.</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool Clear(System.Windows.Forms.Control renderWindow, System.Drawing.Color color)
        {
            if (device == null)
                return false;

            // Check for alt+tab
            try
            {
                device.TestCooperativeLevel();
            }
            catch (DeviceLostException)
            {
            }
            catch (DeviceNotResetException)
            {
                Reset();
            }

            // Find the swap chain
            SetActiveSwapChain(renderWindow);

            Surface pBackBuffer = lSwapChains[ActiveChain].Chain.GetBackBuffer(0, BackBufferType.Mono);

            // set it as our current back buffer
            device.SetRenderTarget(0, pBackBuffer);

            //Clear the backbuffer to a color 
            device.Clear(ClearFlags.Target, color, 1.0f, 0);


            pBackBuffer.Dispose();

            return true;
        }

        /// <summary>
        /// Clears the screen to a specific color.
        /// </summary>
        /// <param name="red">Red component of the color (0-255).</param>
        /// <param name="green">Green component of the color (0-255).</param>
        /// <param name="blue">Blue component of the color (0-255).</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool Clear(Int32 red, Int32 green, Int32 blue)
        {
            return Clear(lSwapChains[ActiveChain].renderWindow, red, green, blue);
        }

        /// <summary>
        /// Clears the screen to a specific color.
        /// </summary>
        /// <param name="color">What color to clear the background to.</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool Clear(System.Drawing.Color color)
        {
            return Clear(lSwapChains[ActiveChain].renderWindow, color);
        }

        /// <summary>
        /// Clears the screen to a specific color.
        /// </summary>
        /// <param name="renderWindow">The window we want to clear.</param>
        /// <param name="red">Red component of the color (0-255).</param>
        /// <param name="green">Green component of the color (0-255).</param>
        /// <param name="blue">Blue component of the color (0-255).</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool Clear(System.Windows.Forms.Control renderWindow, Int32 red, Int32 green, Int32 blue)
        {
            return Clear(renderWindow, System.Drawing.Color.FromArgb(red, green, blue));
        }

        /// <summary>
        /// Begins the Device.
        /// </summary>
        /// <returns>true if successful, false otherwise.</returns>
        public bool DeviceBegin()
        {
            if (device == null)
                return false;

            try
            {
                device.BeginScene();
            }
            catch (InvalidCallException)
            {
                DialogResult r = MessageBox.Show("Failed to begin device scene.", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Begins the Sprite (MUST be called between DeviceBegin() and DeviceEnd()!).
        /// </summary>
        /// <returns>true if successful, false otherwise.</returns>
        public bool SpriteBegin()
        {
            if (sprite == null)
                return false;

            try
            {
                sprite.Begin(SpriteFlags.AlphaBlend);
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to begin sprite scene.", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Begins the Line.
        /// </summary>
        /// <returns>true if successful, false otherwise.</returns>
        public bool LineBegin()
        {
            if (line == null)
                return false;

            try
            {
                line.Begin();
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to begin sprite scene.", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Ends the Device.
        /// </summary>
        /// <returns>true if successful, false otherwise.</returns>
        public bool DeviceEnd()
        {
            if (device == null)
                return false;

            try
            {
                device.EndScene();
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to end device scene.", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Ends the Sprite (MUST be called between DeviceBegin() and DeviceEnd()!).
        /// </summary>
        /// <returns>true if successful, false otherwise.</returns>
        public bool SpriteEnd()
        {
            if (sprite == null)
                return false;

            try
            {
                sprite.End();
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to end sprite scene.", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Ends the Line.
        /// </summary>
        /// <returns>true if successful, false otherwise.</returns>
        public bool LineEnd()
        {
            if (line == null)
                return false;

            try
            {
                line.End();
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to end line scene.", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Present to the screen.
        /// </summary>
        /// <returns>true if successful, false otherwise.</returns>
        public bool Present()
        {
            return Present(lSwapChains[ActiveChain].renderWindow);
        }
        
        /// <summary>
        /// Present to the screen.
        /// </summary>
        /// <param name="renderWindow">The window we want to present.</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool Present(System.Windows.Forms.Control renderWindow)
        {
            if (device == null)
                return false;

            // Find the swap chain
            SetActiveSwapChain(renderWindow);

            try
            {
                //device.Present();
                lSwapChains[ActiveChain].Chain.Present();
            }
            catch (Exception)
            {
                DialogResult r;
                r = MessageBox.Show("Failed to present.", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Draws a colored rectangle to the screen.
        /// Does not support alpha
        /// </summary>
        /// <param name="rect">The rectangle holding the area to fill.</param>
        /// <param name="color">The color of the rect we are going to draw.</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool DrawRect(System.Drawing.Rectangle rect, System.Drawing.Color color)
        {
            if (device == null)
                return false;

            System.Drawing.Rectangle[] regions = new System.Drawing.Rectangle[1];
            regions[0] = rect;

            device.Clear(ClearFlags.Target, color, 1.0f, 0, regions);

            return true;
        }

        /// <summary>
        /// Draws a colored rectangle to the screen.
        /// Does not support alpha
        /// </summary>
        /// <param name="rect">The rectangle holding the area to fill.</param>
        /// <param name="red">Red component of the color (0-255).</param>
        /// <param name="green">Green component of the color (0-255).</param>
        /// <param name="blue">Blue component of the color (0-255).</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool DrawRect(System.Drawing.Rectangle rect, Int32 red, Int32 green, Int32 blue)
        {
            return DrawRect(rect, System.Drawing.Color.FromArgb(red, green, blue));
        }

        /// <summary>
        /// Draws a colored rectangle to the screen with a hollow middle.
        /// Does not support alpha
        /// </summary>
        /// <param name="rect">The rectangle holding the area to draw.</param>
        /// <param name="color">The color of the rect we are going to draw.</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool DrawEmptyRect(System.Drawing.Rectangle rect, System.Drawing.Color color)
        {
            if (line == null)
                return false;

            Vector2[] verts = new Vector2[2];

            LineBegin();
            //Top
            verts[0].X = rect.Left;
            verts[0].Y = rect.Top;

            verts[1].X = rect.Right;
            verts[1].Y = rect.Top;

            line.Draw(verts, color);

            //Bottom
            verts[0].X = rect.Left;
            verts[0].Y = rect.Bottom;

            verts[1].X = rect.Right;
            verts[1].Y = rect.Bottom;

            line.Draw(verts, color);

            //Left
            verts[0].X = rect.Left;
            verts[0].Y = rect.Top;

            verts[1].X = rect.Left;
            verts[1].Y = rect.Bottom;

            line.Draw(verts, color);

            //Right
            verts[0].X = rect.Right;
            verts[0].Y = rect.Top;

            verts[1].X = rect.Right;
            verts[1].Y = rect.Bottom;

            line.Draw(verts, color);
            LineEnd();
            return true;
        }

        /// <summary>
        /// Draws a colored rectangle to the screen with a hollow middle.
        /// Does not support alpha
        /// </summary>
        /// <param name="rect">The rectangle holding the area to draw.</param>
        /// <param name="color">The color of the rect we are going to draw.</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool DrawEmptyRect(System.Drawing.Rectangle rect, Int32 red, Int32 green, Int32 blue)
        {
            return DrawEmptyRect(rect, System.Drawing.Color.FromArgb(red, green, blue));
        }

        /// <summary>
        /// Draws a colored rectangle to the screen with a hollow middle.
        /// Does not support alpha
        /// </summary>
        /// <param name="rect">The rectangle holding the area to draw.</param>
        /// <param name="color">The color of the rect we are going to draw.</param>
        /// <param name="width">The width of the lines that comprise the rect.</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool DrawEmptyRect(System.Drawing.Rectangle rect, System.Drawing.Color color,float width)
        {
            bool thereturn;
            float oldwidth = line.Width;

            line.Width = width;
            thereturn = DrawEmptyRect(rect, color);
            line.Width = oldwidth;

            return thereturn;
        }

        /// <summary>
        /// Draws a colored rectangle to the screen with a hollow middle.
        /// Does not support alpha
        /// </summary>
        /// <param name="rect">The rectangle holding the area to draw.</param>
        /// <param name="color">The color of the rect we are going to draw.</param>
        /// <param name="width">The width of the lines that comprise the rect.</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool DrawEmptyRect(System.Drawing.Rectangle rect, Int32 red, Int32 green, Int32 blue, float width)
        {
            return DrawEmptyRect( rect, System.Drawing.Color.FromArgb(red, green, blue), width);
        }

        /// <summary>
        /// Draws a colored line to the screen.
        /// Does not support alpha
        /// </summary>
        /// <param name="x1">Starting X position.</param>
        /// <param name="y1">Starting Y position.</param>
        /// <param name="x2">Ending X position.</param>
        /// <param name="y2">Ending Y position.</param>
        /// <param name="color">The color of the rect we are going to draw.</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool DrawLine(int x1, int y1, int x2, int y2, System.Drawing.Color color)
        {
            if (line == null)
                return false;

            Vector2[] verts = new Vector2[2];

            //Top
            verts[0].X = x1;
            verts[0].Y = y1;

            verts[1].X = x2;
            verts[1].Y = y2;

            line.Draw(verts, color); 
            
            return true;
        }

        /// <summary>
        /// Draws a colored line to the screen.
        /// Does not support alpha
        /// </summary>
        /// <param name="x1">Starting X position.</param>
        /// <param name="y1">Starting Y position.</param>
        /// <param name="x2">Ending X position.</param>
        /// <param name="y2">Ending Y position.</param>
        /// <param name="red">Red component of the color (0-255).</param>
        /// <param name="green">Green component of the color (0-255).</param>
        /// <param name="blue">Blue component of the color (0-255).</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool DrawLine(int x1, int y1, int x2, int y2, Int32 red, Int32 green, Int32 blue)
        {
            return DrawLine(x1, y1, x2, y2, System.Drawing.Color.FromArgb(red, green, blue));
        }

        /// <summary>
        /// Draws a colored line to the screen.
        /// Does not support alpha
        /// </summary>
        /// <param name="x1">Starting X position.</param>
        /// <param name="y1">Starting Y position.</param>
        /// <param name="x2">Ending X position.</param>
        /// <param name="y2">Ending Y position.</param>
        /// <param name="color">The color of the rect we are going to draw.</param>
        /// <param name="width">The width of the line.</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool DrawLine(int x1, int y1, int x2, int y2, System.Drawing.Color color, float width)
        {
            bool thereturn;
            float oldwidth = line.Width;

            line.Width = width;            

            thereturn = DrawLine(x1, y1, x2, y2, color);
            line.Width = oldwidth;

            return thereturn;
        }

        /// <summary>
        /// Draws a colored line to the screen.
        /// Does not support alpha
        /// </summary>
        /// <param name="x1">Starting X position.</param>
        /// <param name="y1">Starting Y position.</param>
        /// <param name="x2">Ending X position.</param>
        /// <param name="y2">Ending Y position.</param>
        /// <param name="red">Red component of the color (0-255).</param>
        /// <param name="green">Green component of the color (0-255).</param>
        /// <param name="blue">Blue component of the color (0-255).</param>
        /// <param name="width">The width of the line.</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool DrawLine(int x1, int y1, int x2, int y2, Int32 red, Int32 green, Int32 blue, float width)
        {
            return DrawLine(x1, y1, x2, y2, System.Drawing.Color.FromArgb(red, green, blue), width);
        }

        /// <summary>
        /// Draws colored text to the screen.
        /// </summary>
        /// <param name="text">Text to print to the screen.</param>
        /// <param name="x">X position of text.</param>
        /// <param name="y">Y position of text.</param>
        /// <param name="color">The color of the rect we are going to draw.</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool DrawText(string text, int x, int y, System.Drawing.Color color)
        {
            if (font == null)
                return false;

            font.DrawText(sprite, text, new System.Drawing.Point(x, y), color);

            return true;
        }

        /// <summary>
        /// Draws colored text to the screen.
        /// </summary>
        /// <param name="text">Text to print to the screen.</param>
        /// <param name="x">X position of text.</param>
        /// <param name="y">Y position of text.</param>
        /// <param name="red">Red component of the color (0-255).</param>
        /// <param name="green">Green component of the color (0-255).</param>
        /// <param name="blue">Blue component of the color (0-255).</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool DrawText(string text, int x, int y, Int32 red, Int32 green, Int32 blue)
        {
            return DrawText(text, x, y, System.Drawing.Color.FromArgb(red, green, blue));
        }

        /// <summary>
        /// Shutsdown direct X releacing all the memory used by the swapchains
        /// </summary>
        /// <returns>true if successful, false otherwise.</returns>
        public bool Shutdown()
        {
            for(int i = 0; i < lSwapChains.Count; i++)
            {
                lSwapChains[i].Release();
            }
            return true;
        }

    } // end ManagedDirect3D
    public class ManagedSwapChain
    {
        PresentParameters presentParams = null;
        Control RenderWindow = null;

        public Control renderWindow
        {
            get { return RenderWindow; }
            set { RenderWindow = value; }
        }
        SwapChain chain = null;

        public SwapChain Chain
        {
            get { return chain; }
        }

        public ManagedSwapChain(Device device, PresentParameters param)
        {
            presentParams = param;
            renderWindow = presentParams.DeviceWindow;
            chain = new SwapChain(device, presentParams);
        }
        public void Reset(Device device)
        {
            chain.Dispose();
            chain = new SwapChain(device, presentParams);
        }
        public void Reset(Device device, PresentParameters param)
        {
            presentParams = param;
            renderWindow = presentParams.DeviceWindow;
            chain.Dispose();
            chain = new SwapChain(device, param);
        }
        public void Release()
        {
            chain.Dispose();
            chain = null;
            presentParams = null;
        }
    }
}
