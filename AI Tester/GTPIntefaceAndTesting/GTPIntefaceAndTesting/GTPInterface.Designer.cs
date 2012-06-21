namespace GTPIntefaceAndTesting
{
    partial class GTPInterface
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.nineBoard = new System.Windows.Forms.Panel();
            this.getComputerMove = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.computerOutput = new System.Windows.Forms.Panel();
            this.winRateDisplay = new System.Windows.Forms.Panel();
            this.SuspendLayout();
            // 
            // nineBoard
            // 
            this.nineBoard.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)));
            this.nineBoard.BackColor = System.Drawing.Color.Yellow;
            this.nineBoard.Location = new System.Drawing.Point(12, 12);
            this.nineBoard.Name = "nineBoard";
            this.nineBoard.Size = new System.Drawing.Size(400, 515);
            this.nineBoard.TabIndex = 0;
            this.nineBoard.Paint += new System.Windows.Forms.PaintEventHandler(this.nineBoard_Paint);
            this.nineBoard.MouseClick += new System.Windows.Forms.MouseEventHandler(this.nineBoard_MouseClick);
            // 
            // getComputerMove
            // 
            this.getComputerMove.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.getComputerMove.Location = new System.Drawing.Point(12, 533);
            this.getComputerMove.Name = "getComputerMove";
            this.getComputerMove.Size = new System.Drawing.Size(164, 23);
            this.getComputerMove.TabIndex = 1;
            this.getComputerMove.Text = "Get Computer Move";
            this.getComputerMove.UseVisualStyleBackColor = true;
            this.getComputerMove.Click += new System.EventHandler(this.getComputerMove_Click);
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.button1.Location = new System.Drawing.Point(182, 533);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(164, 23);
            this.button1.TabIndex = 3;
            this.button1.Text = "Get Board Layout";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // computerOutput
            // 
            this.computerOutput.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.computerOutput.Location = new System.Drawing.Point(418, 12);
            this.computerOutput.Name = "computerOutput";
            this.computerOutput.Size = new System.Drawing.Size(541, 515);
            this.computerOutput.TabIndex = 4;
            this.computerOutput.Paint += new System.Windows.Forms.PaintEventHandler(this.computerOutput_Paint);
            // 
            // winRateDisplay
            // 
            this.winRateDisplay.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.winRateDisplay.Location = new System.Drawing.Point(966, 13);
            this.winRateDisplay.Name = "winRateDisplay";
            this.winRateDisplay.Size = new System.Drawing.Size(269, 514);
            this.winRateDisplay.TabIndex = 5;
            this.winRateDisplay.Paint += new System.Windows.Forms.PaintEventHandler(this.winRateDisplay_Paint);
            // 
            // GTPInterface
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1247, 568);
            this.Controls.Add(this.winRateDisplay);
            this.Controls.Add(this.computerOutput);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.getComputerMove);
            this.Controls.Add(this.nineBoard);
            this.Name = "GTPInterface";
            this.Text = "GTP Interface";
            this.Click += new System.EventHandler(this.GTPInterface_Click);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel nineBoard;
        private System.Windows.Forms.Button getComputerMove;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Panel computerOutput;
        private System.Windows.Forms.Panel winRateDisplay;
    }
}

