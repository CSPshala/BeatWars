using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace ParticleFX
{
    public class Particle
    {
        // Data Members
        public Vector2 Position;
        public Vector2 Velocity;
        public Color Overlay;
        public float Scale;
        public float Rotation;
        public float CurrentLife;
        public float Duration;
        public byte ImageID;

        // Delta Values for changing over time
        public Vector2 VelocityDelta;
        public float RedDelta;
        public float GreenDelta;
        public float BlueDelta;
        public float AlphaDelta;
        public float ScaleDelta;
        public float RotationDelta;

        Vector2 Origin = new Vector2();

        // Constructor
        public Particle()
        {
            // Emitter should set the values!
        }

        // Logic Methods
        public void Update(PerformanceCounter GameTimer, Vector2 Gravity, float GravitationalPull)
        {
            // Update Color
            Vector4 NewColor = Overlay.ToVector4();
            NewColor.X += RedDelta * GameTimer.ElapsedTime;
            NewColor.Y += GreenDelta * GameTimer.ElapsedTime;
            NewColor.Z += BlueDelta * GameTimer.ElapsedTime;
            NewColor.W += AlphaDelta * GameTimer.ElapsedTime;
            Overlay = new Color(NewColor);

            // Update Velocity
            Velocity.X += VelocityDelta.X * GameTimer.ElapsedTime;
            Velocity.Y += VelocityDelta.Y * GameTimer.ElapsedTime;

            Vector2 DeltaPosWell = Gravity - Position;
            DeltaPosWell.Normalize();
            DeltaPosWell = DeltaPosWell * (GravitationalPull * GameTimer.ElapsedTime);
            Velocity = Velocity + DeltaPosWell;

            // Update Position
            Position = Position + (Velocity * GameTimer.ElapsedTime) ;

            // Update Scale and Velocity
            Scale += ScaleDelta * GameTimer.ElapsedTime;
            Rotation += RotationDelta * GameTimer.ElapsedTime;

            // Update the current life span
            CurrentLife += GameTimer.ElapsedTime;
        }
        public void Render(SpriteBatch Batch, List<ParticleTexture> Textures)
        {
            if (ImageID >= 0 && ImageID < Textures.Count)
            {
                Origin.X = ((float)(Textures[ImageID].Texture.Bounds.Width) * 0.5f);
                Origin.Y = ((float)(Textures[ImageID].Texture.Bounds.Height) * 0.5f);
                Batch.Draw(Textures[ImageID].Texture, Position , Textures[ImageID].Texture.Bounds, Overlay, Rotation, (Origin), Scale, SpriteEffects.None, 1.0f);
            }
        }
    }
}
