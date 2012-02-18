using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace ParticleFX
{
    public class ParticleEmitter
    {
        // Data Members
        public string Name;
        public Rectangle SpawnRange;
        public Vector2 Gravity;
        public float GravitationalPull;
        public float ParticleDuration;
        public float ParticleDurationMax;
        public float LifeSpan;
        public float SpawnRate;
        private float TimeLastUpdate;
        public short MaxParticles;
        public short NumParticlesToSpit;

        // Options
        public bool RandomStartVelocityX;
        public bool RandomStartVelocityY;
        public bool RandomEndVelocityX;
        public bool RandomEndVelocityY;

        // Particle Data
        public List<ParticleTexture> ParticleTextures;
        public List<Particle> AliveParticles;
        public List<Particle> DeadParticles;
        public int ParticleCountTotal
        {
            get { return AliveParticles.Count + DeadParticles.Count; }
        }
        public int ParticleCountRendering
        {
            get { return AliveParticles.Count; }
        }

        // Start & End Values
        public Color StartColor;
        public Color EndColor;
        public Blend SourceBlend;
        public Blend DestinationBlend;
        public Vector2 StartVelocity;
        public Vector2 EndVelocity;
        public float StartScale;
        public float EndScale;
        public float StartRotation;
        public float EndRotation;

        // Constructor
        public ParticleEmitter()
        {
            Name = "";
            LifeSpan = 5.0f;
            MaxParticles = 50; 
            SpawnRate = 0.01f;
            SpawnRange = new Rectangle(248, 248, 32, 32);
            NumParticlesToSpit = 1;
            Gravity = Vector2.Zero;
            GravitationalPull = 0.0f;

            ParticleTextures = new List<ParticleTexture>();
            AliveParticles = new List<Particle>();
            DeadParticles = new List<Particle>();

            for (short i = 0; i < MaxParticles; ++i)
                DeadParticles.Add(new Particle());

            StartColor = Color.DarkOrange;
            EndColor = Color.Firebrick;
            SourceBlend = Blend.SourceAlpha;
            DestinationBlend = Blend.One;
            StartVelocity = new Vector2(0.0f, -200.0f);
            EndVelocity = Vector2.Zero;
            StartScale = 1.0f;
            EndScale = 0.0f;
            StartRotation = 0.0f;
            EndRotation = 0.0f;
            ParticleDuration = 0.5f;
            ParticleDurationMax = 1.0f;
        }

        // Logic Methods
        public void Update(PerformanceCounter GameTimer)
        {
            if (ParticleTextures.Count > 0)
            {
                int i; // Used for indexing

                // Update Alive Particles
                for (i = 0; i < AliveParticles.Count; ++i)
                {
                    AliveParticles[i].Update(GameTimer, Gravity, GravitationalPull);

                    if (AliveParticles[i].CurrentLife > AliveParticles[i].Duration)
                    {
                        DeadParticles.Add(AliveParticles[i]);
                        AliveParticles.RemoveAt(i);
                        --i;
                    }
                }

                // Time to spit out more particles?
                if (GameTimer.GameTime - TimeLastUpdate > SpawnRate)
                {
                    for (i = 0; i < NumParticlesToSpit; ++i)
                    {
                        if(AliveParticles.Count < MaxParticles)
                            RecycleParticle();
                        else
                            break;
                    }
                    TimeLastUpdate = GameTimer.GameTime;
                }
            }
        }
        Random Rand = new Random();
        public void RecycleParticle()
        {
            if (DeadParticles.Count > 0)
            {
                int Index = AliveParticles.Count;

                // Recycle the particle
                AliveParticles.Add(DeadParticles[0]);
                DeadParticles.RemoveAt(0);

                // Set up base attributes
                AliveParticles[Index].Position.X = (float)Rand.Next(SpawnRange.Left, SpawnRange.Right);
                AliveParticles[Index].Position.Y = (float)Rand.Next(SpawnRange.Top, SpawnRange.Bottom);
                AliveParticles[Index].ImageID = (byte)Rand.Next(0, ParticleTextures.Count);

                if (RandomStartVelocityX)
                    AliveParticles[Index].Velocity.X = (float)Rand.Next(-300, 300) + (float)Rand.NextDouble();
                else
                    AliveParticles[Index].Velocity.X = StartVelocity.X;

                if (RandomStartVelocityY)
                    AliveParticles[Index].Velocity.Y = (float)Rand.Next(-300, 300) + (float)Rand.NextDouble();
                else
                    AliveParticles[Index].Velocity.Y = StartVelocity.Y;

                AliveParticles[Index].Overlay = StartColor;
                AliveParticles[Index].Rotation = StartRotation;
                AliveParticles[Index].Scale = StartScale;

                int DurRand = Rand.Next((int)ParticleDuration, (int)ParticleDurationMax);
                AliveParticles[Index].Duration = ((float)DurRand) + ((float)Rand.NextDouble());

                AliveParticles[Index].CurrentLife = 0.0f;
                
                // Set up the delta values
                Vector4 StartColorVector = StartColor.ToVector4();
                Vector4 EndColorVector = EndColor.ToVector4();
                AliveParticles[Index].RedDelta = EndColorVector.X - StartColorVector.X;
                AliveParticles[Index].GreenDelta = EndColorVector.Y - StartColorVector.Y;
                AliveParticles[Index].BlueDelta = EndColorVector.Z - StartColorVector.Z;
                AliveParticles[Index].AlphaDelta = EndColorVector.W - StartColorVector.W;
                AliveParticles[Index].RotationDelta = EndRotation - StartRotation;
                AliveParticles[Index].ScaleDelta = EndScale - StartScale;
                AliveParticles[Index].VelocityDelta = new Vector2(EndVelocity.X - StartVelocity.X, EndVelocity.Y - StartVelocity.Y);
            }
        }
        public void Render(SpriteBatch Batch)
        {
            BlendState BS = new BlendState();
            BS.AlphaSourceBlend = SourceBlend;
            BS.AlphaDestinationBlend = DestinationBlend;
            BS.ColorSourceBlend = SourceBlend;
            BS.ColorDestinationBlend = DestinationBlend;

            for (int i = 0; i < AliveParticles.Count; ++i)
            {
                Batch.Begin(SpriteSortMode.Deferred, BS);
                AliveParticles[i].Render(Batch, ParticleTextures);
                Batch.End();
            }
        }

        // Overrides
        public override string ToString()
        {
            return Name;
        }
    }
}