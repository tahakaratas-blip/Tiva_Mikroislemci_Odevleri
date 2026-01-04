namespace SERİ
{
    partial class MainForm
    {
        /// <summary>
        /// Designer variable used to keep track of non-visual components.
        /// </summary>
        private System.ComponentModel.IContainer components = null;
        
        /// <summary>
        /// Disposes resources used by the form.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing) {
                if (components != null) {
                    components.Dispose();
                }
            }
            base.Dispose(disposing);
        }
        
        /// <summary>
        /// This method is required for Windows Forms designer support.
        /// Do not change the method contents inside the source code editor. The Forms designer might
        /// not be able to load this method if it was changed manually.
        /// </summary>
        private void InitializeComponent()
        {
        	this.components = new System.ComponentModel.Container();
        	this.txtSaat = new System.Windows.Forms.TextBox();
        	this.btnSaatGonder = new System.Windows.Forms.Button();
        	this.btnBaglan = new System.Windows.Forms.Button();
        	this.btnKES = new System.Windows.Forms.Button();
        	this.txtMetin = new System.Windows.Forms.TextBox();
        	this.btnMetinGonder = new System.Windows.Forms.Button();
        	this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
        	this.SuspendLayout();
        	// 
        	// txtSaat
        	// 
        	this.txtSaat.Location = new System.Drawing.Point(17, 59);
        	this.txtSaat.Name = "txtSaat";
        	this.txtSaat.Size = new System.Drawing.Size(158, 22);
        	this.txtSaat.TabIndex = 0;
        	// 
        	// btnSaatGonder
        	// 
        	this.btnSaatGonder.Location = new System.Drawing.Point(209, 48);
        	this.btnSaatGonder.Name = "btnSaatGonder";
        	this.btnSaatGonder.Size = new System.Drawing.Size(120, 65);
        	this.btnSaatGonder.TabIndex = 1;
        	this.btnSaatGonder.Text = "GÖNDER";
        	this.btnSaatGonder.UseVisualStyleBackColor = true;
        	this.btnSaatGonder.Click += new System.EventHandler(this.BtnSaatGonderClick);
        	// 
        	// btnBaglan
        	// 
        	this.btnBaglan.Location = new System.Drawing.Point(368, 50);
        	this.btnBaglan.Name = "btnBaglan";
        	this.btnBaglan.Size = new System.Drawing.Size(79, 63);
        	this.btnBaglan.TabIndex = 2;
        	this.btnBaglan.Text = "Port Aç";
        	this.btnBaglan.UseVisualStyleBackColor = true;
        	this.btnBaglan.Click += new System.EventHandler(this.BtnBaglanClick);
        	// 
        	// btnKES
        	// 
        	this.btnKES.Location = new System.Drawing.Point(491, 50);
        	this.btnKES.Name = "btnKES";
        	this.btnKES.Size = new System.Drawing.Size(99, 63);
        	this.btnKES.TabIndex = 3;
        	this.btnKES.Text = "KAPAT";
        	this.btnKES.UseVisualStyleBackColor = true;
        	this.btnKES.Click += new System.EventHandler(this.BtnKESClick);
        	// 
        	// txtMetin
        	// 
        	this.txtMetin.Location = new System.Drawing.Point(27, 201);
        	this.txtMetin.Name = "txtMetin";
        	this.txtMetin.Size = new System.Drawing.Size(134, 22);
        	this.txtMetin.TabIndex = 4;
        	// 
        	// btnMetinGonder
        	// 
        	this.btnMetinGonder.Location = new System.Drawing.Point(274, 191);
        	this.btnMetinGonder.Name = "btnMetinGonder";
        	this.btnMetinGonder.Size = new System.Drawing.Size(127, 61);
        	this.btnMetinGonder.TabIndex = 5;
        	this.btnMetinGonder.Text = "MESAJ GÖNDER";
        	this.btnMetinGonder.UseVisualStyleBackColor = true;
        	this.btnMetinGonder.Click += new System.EventHandler(this.BtnMetinGonderClick);
        	// 
        	// serialPort1
        	// 
        	this.serialPort1.PortName = "COM12";
        	// 
        	// MainForm
        	// 
        	this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
        	this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
        	this.ClientSize = new System.Drawing.Size(654, 414);
        	this.Controls.Add(this.btnMetinGonder);
        	this.Controls.Add(this.txtMetin);
        	this.Controls.Add(this.btnKES);
        	this.Controls.Add(this.btnBaglan);
        	this.Controls.Add(this.btnSaatGonder);
        	this.Controls.Add(this.txtSaat);
        	this.Name = "MainForm";
        	this.Text = "SERİ";
        	this.ResumeLayout(false);
        	this.PerformLayout();
        }
        
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Button btnMetinGonder;
        private System.Windows.Forms.TextBox txtMetin;
        private System.Windows.Forms.Button btnKES;
        private System.Windows.Forms.Button btnBaglan;
        private System.Windows.Forms.Button btnSaatGonder;
        private System.Windows.Forms.TextBox txtSaat;
    }
}