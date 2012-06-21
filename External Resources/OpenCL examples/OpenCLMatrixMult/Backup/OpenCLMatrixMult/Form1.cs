using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace OpenCLMatrixMult
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        CLMatrixMult CLmult;
        private void Form1_Load(object sender, EventArgs e)
        {
            CLmult = new CLMatrixMult();

        }

        private void btnMultMatrix1_Click(object sender, EventArgs e)
        {
            int P = (int)numM.Value, Q = (int)numQ.Value, R = (int)numR.Value;
            lblErrLocals.Text = ""; lblCPUtime.Text = ""; lblErrNoLocals.Text = ""; lblMultLocals.Text = ""; lblMultNoLocals.Text = "";
            Application.DoEvents();

            //Matrixes
            float[,] m1 = new float[P, Q];
            float[,] m2 = new float[Q, R];
            Random rnd = new Random();

            //Random population
            for (int i = 0; i < P; i++)
            {
                for (int j = 0; j < Q; j++)
                {
                    m1[i, j] = (float)rnd.NextDouble();
                }
            }
            for (int i = 0; i < Q; i++)
            {
                for (int j = 0; j < R; j++)
                {
                    m2[i, j] = (float)rnd.NextDouble();
                }
            }

            //Diagnostics
            System.Diagnostics.Stopwatch sw1 = new System.Diagnostics.Stopwatch();
            System.Diagnostics.Stopwatch sw2 = new System.Diagnostics.Stopwatch();
            System.Diagnostics.Stopwatch sw3 = new System.Diagnostics.Stopwatch();

            //Multiplication using __locals
            sw1.Start();
            float[,] m3 = CLmult.MultiplyLocals(m1, m2);
            sw1.Stop();
            lblMultLocals.Text = sw1.Elapsed.ToString(); Application.DoEvents();

            //Multiplication not using __locals
            sw2.Start();
            float[,] m4 = CLmult.MultiplyNoLocals(m1, m2);
            sw2.Stop();
            lblMultNoLocals.Text = sw2.Elapsed.ToString(); Application.DoEvents();

            //CPU Multiplication
            sw3.Start();
            double[,] m5 = CLmult.MultiplyNoOpenCL(m1, m2);
            sw3.Stop();
            lblCPUtime.Text = sw3.Elapsed.ToString();

            //Precision test, no locals
            double dif = 0;
            for (int i = 0; i < P; i++)
            {
                for (int j = 0; j < R; j++)
                {
                    dif += (m4[i, j] - m5[i, j]) * (m4[i, j] - m5[i, j]);
                }
            }
            dif = Math.Sqrt(dif) / ((double)P * (double)R);
            lblErrNoLocals.Text = dif.ToString();

            //Precision test, locals
            dif = 0;
            for (int i = 0; i < P; i++)
            {
                for (int j = 0; j < R; j++)
                {
                    dif += (m3[i, j] - m5[i, j]) * (m3[i, j] - m5[i, j]);
                }
            }
            dif = Math.Sqrt(dif) / ((double)P * (double)R);
            lblErrLocals.Text = dif.ToString();

        }

        private void numM_Leave(object sender, EventArgs e)
        {
            NumericUpDown numUpDown = (NumericUpDown)sender;
            numUpDown.Value = Math.Ceiling(numUpDown.Value / 16) * 16;
        }

    }
}
