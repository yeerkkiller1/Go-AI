using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace OpenCLFilter
{
    public partial class frmCfgFilter : Form
    {
        public frmCfgFilter()
        {
            InitializeComponent();
        }

        int FilterSize = 7;
        TextBox[] txtR, txtG, txtB;

        /// <summary>Returns red, green and blue filters. </summary>
        public float[] GetFilters()
        {
            float[] Filters = new float[3 * FilterSize * FilterSize];

            for (int i = 0; i < FilterSize; i++)
            {
                for (int j = 0; j < FilterSize; j++)
                {
                    float r, g, b;
                    float.TryParse(txtR[i * FilterSize + j].Text, out r);
                    float.TryParse(txtG[i * FilterSize + j].Text, out g);
                    float.TryParse(txtB[i * FilterSize + j].Text, out b);

                    Filters[3*(i * FilterSize + j)] = r; //red component
                    Filters[3*(i * FilterSize + j)+1] = g; //green
                    Filters[3*(i * FilterSize + j)+2] = b; //blue
                }
            }

            return Filters;
        }

        #region Textboxes management
        private void frmCfgFilter_Load(object sender, EventArgs e)
        {
            txtR = new TextBox[FilterSize * FilterSize];
            txtG = new TextBox[FilterSize * FilterSize];
            txtB = new TextBox[FilterSize * FilterSize];

            float[] gaussian = new float[] 
            {
                0,0, 0, 0, 0,0,0,
                0,2, 4, 5, 4,2,0,
                0,4, 9,12, 9,4,0,
                0,5,12,15,12,5,0,
                0,4, 9,12, 9,4,0,
                0,2, 4, 5, 4,2,0,
                0,0, 0, 0, 0,0,0
            };
            for (int i = 0; i < gaussian.Length; i++) gaussian[i] /= 159;

            for (int i = 0; i < FilterSize; i++)
            {
                for (int j = 0; j < FilterSize; j++)
                {
                    txtR[i * FilterSize + j] = new TextBox();
                    txtR[i * FilterSize + j].Width = 45; txtR[i * FilterSize + j].Top = 19 + 26 * i; 
                    txtR[i * FilterSize + j].Left = 6 + 50 * j;
                    txtR[i * FilterSize + j].Text = gaussian[i * FilterSize + j].ToString();
                    txtR[i * FilterSize + j].Font = new Font("Arial", 9, FontStyle.Regular);
                    txtR[i * FilterSize + j].Leave += new EventHandler(frmCfgFilter_Leave);
                    gbR.Controls.Add(txtR[i * FilterSize + j]);

                    txtG[i * FilterSize + j] = new TextBox();
                    txtG[i * FilterSize + j].Width = 45; txtG[i * FilterSize + j].Top = 19 + 26 * i;
                    txtG[i * FilterSize + j].Left = 6 + 50 * j;
                    txtG[i * FilterSize + j].Text = gaussian[i * FilterSize + j].ToString();
                    txtG[i * FilterSize + j].Font = new Font("Arial", 9, FontStyle.Regular);
                    txtG[i * FilterSize + j].Leave += new EventHandler(frmCfgFilter_Leave);
                    gbG.Controls.Add(txtG[i * FilterSize + j]);

                    txtB[i * FilterSize + j] = new TextBox();
                    txtB[i * FilterSize + j].Width = 45; txtB[i * FilterSize + j].Top = 19 + 26 * i;
                    txtB[i * FilterSize + j].Left = 6 + 50 * j;
                    txtB[i * FilterSize + j].Text = gaussian[i * FilterSize + j].ToString();
                    txtB[i * FilterSize + j].Font = new Font("Arial", 9, FontStyle.Regular);
                    txtB[i * FilterSize + j].Leave += new EventHandler(frmCfgFilter_Leave);
                    gbB.Controls.Add(txtB[i * FilterSize + j]);

                }
            }

            WriteCodes();
        }

        void frmCfgFilter_Leave(object sender, EventArgs e)
        {
            TextBox t = (TextBox)sender;
            float x;
            float.TryParse(t.Text, out x);
            t.Text = x.ToString();
            WriteCodes();
        }

        /// <summary>Writes filter codes to textbox to make them easier to retrieve</summary>
        void WriteCodes()
        {
            string sR = "", sG = "", sB = "";
            for (int i = 0; i < FilterSize; i++)
            {
                for (int j = 0; j < FilterSize; j++)
                {
                    sR += txtR[i * FilterSize + j].Text;
                    sG += txtG[i * FilterSize + j].Text;
                    sB += txtB[i * FilterSize + j].Text;
                    if (j != FilterSize - 1)
                    {
                        sR += " ";
                        sG += " ";
                        sB += " ";
                    }
                }
                if (i != FilterSize - 1)
                {
                    sR += ";";
                    sG += ";";
                    sB += ";";
                }
            }
            txtRCode.Text = sR;
            txtGCode.Text = sG;
            txtBCode.Text = sB;

            CalcMagnitudes();
        }

        /// <summary>Reads filter codes from textbox</summary>
        void ReadCodes()
        {
            string[] sR = txtRCode.Text.Split(';');
            string[] sG = txtRCode.Text.Split(';');
            string[] sB = txtRCode.Text.Split(';');
            if (sR.Length != FilterSize && sG.Length != FilterSize || sB.Length != FilterSize)
            {
                MessageBox.Show("Filter should have "+FilterSize.ToString() +" rows");
                WriteCodes();
                return;
            }

            //Check columns
            for (int i = 0; i < FilterSize; i++)
            {
                string[] sR2 = sR[i].Split();
                string[] sG2 = sG[i].Split();
                string[] sB2 = sB[i].Split();
                if (sR2.Length != FilterSize && sG2.Length != FilterSize || sB2.Length != FilterSize)
                {
                    MessageBox.Show("Filter should have " + FilterSize.ToString() + " columns in each row");
                    WriteCodes();
                    return;
                }
            }

            //Parse filter
            for (int i = 0; i < FilterSize; i++)
            {
                string[] sR2 = sR[i].Split();
                string[] sG2 = sR[i].Split();
                string[] sB2 = sR[i].Split();
                for (int j = 0; j < FilterSize; j++)
                {
                    float r, g, b;
                    float.TryParse(sR2[j], out r);
                    float.TryParse(sG2[j], out g);
                    float.TryParse(sB2[j], out b);
                    txtR[i * FilterSize + j].Text = r.ToString();
                    txtG[i * FilterSize + j].Text = g.ToString();
                    txtB[i * FilterSize + j].Text = b.ToString();
                }
            }

            CalcMagnitudes();
        }

        /// <summary>Returns magnitudes of the filters</summary>
        private float[] CalcMagnitudes()
        {
            float[] resp = new float[3];

            for (int i = 0; i < FilterSize; i++)
            {
                for (int j = 0; j < FilterSize; j++)
                {
                    float r, g, b;
                    float.TryParse(txtR[i * FilterSize + j].Text, out r);
                    float.TryParse(txtG[i * FilterSize + j].Text, out g);
                    float.TryParse(txtB[i * FilterSize + j].Text, out b);

                    resp[0] += r; resp[1] += g; resp[2] += b;
                }
            }

            lblAmpR.Text = resp[0].ToString();
            lblAmpG.Text = resp[1].ToString();
            lblAmpB.Text = resp[2].ToString();

            return resp;
        }

        private void txtCode_Leave(object sender, EventArgs e)
        {
            ReadCodes();
        }

        private void btnCopy_Click(object sender, EventArgs e)
        {
            txtGCode.Text = txtRCode.Text;
            txtBCode.Text = txtRCode.Text;
            ReadCodes();
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            string s = "0 0 0 0 0 0 0;0 0 0 0 0 0 0;0 0 0 0 0 0 0;0 0 0 1 0 0 0;0 0 0 0 0 0 0;0 0 0 0 0 0 0;0 0 0 0 0 0 0";
            txtRCode.Text = s;
            txtGCode.Text = txtRCode.Text;
            txtBCode.Text = txtRCode.Text;
            ReadCodes();
        }

        private void btnNorm_Click(object sender, EventArgs e)
        {
            Button btn = (Button)sender;

            float[] Mags = CalcMagnitudes();
            Mags[0] = 1 / Mags[0];
            Mags[1] = 1 / Mags[1];
            Mags[2] = 1 / Mags[2];
            for (int i = 0; i < FilterSize; i++)
            {
                for (int j = 0; j < FilterSize; j++)
                {
                    float r, g, b;
                    float.TryParse(txtR[i * FilterSize + j].Text, out r);
                    float.TryParse(txtG[i * FilterSize + j].Text, out g);
                    float.TryParse(txtB[i * FilterSize + j].Text, out b);

                    if (btn.Name == "btnNormR") txtR[i * FilterSize + j].Text = (r * Mags[0]).ToString();
                    if (btn.Name == "btnNormG") txtG[i * FilterSize + j].Text = (g * Mags[1]).ToString();
                    if (btn.Name == "btnNormB") txtB[i * FilterSize + j].Text = (b * Mags[2]).ToString();
                }
            }
            CalcMagnitudes();
            WriteCodes();
        }
        #endregion

    }
}
