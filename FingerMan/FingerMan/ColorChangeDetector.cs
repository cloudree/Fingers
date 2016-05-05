using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Drawing;

namespace FingerMan
{
    class ColorChangeDetector
    {
        // color detect
        public enum RoughColor
        {
            RC_UNKNOWN = -2,
            RC_NOTCHANGED = -1,

            RC_BASE = -1,
            RC_RED,
            RC_BLUE,
            RC_ORANGE,
            RC_WHITE,
            RC_MAX
        };

        public struct FloatPoint
        {
            public float X;         // 0.0f ~ 1.0f
            public float Y;         // 0.0f ~ 1.0f
            public FloatPoint( float fx, float fy ) { X = fx; Y = fy; }
        };

        //                               Red  Blu  Org  Wht
        private int[] R_GT = new int[] { 128,   0, 128, 197 };
        private int[] R_LT = new int[] { 256,  64, 256, 256 };
        private int[] G_GT = new int[] {   0,  64, 128, 197 };
        private int[] G_LT = new int[] { 192, 192, 256, 256 };
        private int[] B_GT = new int[] {   0, 192,   0, 197 };
        private int[] B_LT = new int[] { 192, 256, 128, 256 };

        private RoughColor getColor( Color col )
        {
            for( int i= (int)RoughColor.RC_BASE+1; i< (int)RoughColor.RC_MAX; i++ )
            {
                if( R_GT[i] <= col.R && col.R <= R_LT[i] && 
                    G_GT[i] <= col.G && col.G <= G_LT[i] && 
                    B_GT[i] <= col.B && col.B <= B_LT[i] )
                {
                    if ((RoughColor)i == RoughColor.RC_RED)
                    {
                        if (col.G > col.B + 48)
                            return RoughColor.RC_ORANGE;
                    }
                    return (RoughColor)i;
                }
            }
            return RoughColor.RC_UNKNOWN;
        }

        // chnage detect
        private RoughColor lastColor;
        private FloatPoint fpt;

        public ColorChangeDetector()
        {
            lastColor = RoughColor.RC_UNKNOWN;
            fpt = new FloatPoint(-1, -1);
        }

        private float range(float f)
        {
            if (f < 0.0f) return 0.0f;
            if (f >= 1.0f) return 0.999f;
            return f;
        }

        public void setPoint(FloatPoint fp)
        {
            lastColor = RoughColor.RC_UNKNOWN;
            fpt.X = range(fp.X);
            fpt.Y = range(fp.Y);
        }

        public void setPoint( float x, float y )
        {
            setPoint(new FloatPoint(x, y));
        }

        public RoughColor getChangedColor(ref Bitmap image)
        {
            Color col = new Color();
            return getChangedColor(ref image, ref col);
        }

        public RoughColor getChangedColor(ref Bitmap image, ref Color col)
        {
            col = image.GetPixel((int)(fpt.X * image.Size.Width), (int)(fpt.Y * image.Size.Height));
            RoughColor rc = getColor(col);

            // not first detect
            if (lastColor != RoughColor.RC_UNKNOWN)
            {
                // not changed color
                if (lastColor == rc)
                    return RoughColor.RC_NOTCHANGED;
            }

            // changed or first detect
            lastColor = rc;
            return rc;
        }

        public RoughColor getColor(ref Bitmap image)
        {
            Color col = image.GetPixel((int)(fpt.X * image.Size.Width), (int)(fpt.Y * image.Size.Height));
            return getColor(col);
        }
    }
}
