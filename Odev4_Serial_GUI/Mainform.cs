using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.IO.Ports;

namespace SERİ
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }
        
        // --- 1. PORT AÇ BUTONU ---
        void BtnBaglanClick(object sender, EventArgs e)
        {
            try
            {
                // Port ayarları (USB kablosunu değiştirdiğinde burayı güncellemelisin)
                serialPort1.PortName = "COM12"; 
                serialPort1.BaudRate = 9600;
                
                if(!serialPort1.IsOpen)
                {
                    serialPort1.Open();
                    
                    // Görselliği ayarla
                    btnBaglan.Enabled = false; // Aç butonunu pasif yap
                    btnKES.Enabled = true;     // Kapat butonunu aktif yap (İsim DÜZELTİLDİ: btnKES)
                    
                    MessageBox.Show("Port Bağlantısı Başarılı!");
                }
            }
            catch(Exception ex)
            {
                MessageBox.Show("Hata: " + ex.Message + "\nLütfen Aygıt Yöneticisi'nden COM numaranı kontrol et.");
            }
        }
        
        // --- 2. KAPAT BUTONU ---
        // Designer dosyasındaki isme uygun hale getirildi: BtnKESClick
        void BtnKESClick(object sender, EventArgs e)
        {
            if(serialPort1.IsOpen)
            {
                serialPort1.Close();
                
                // Görselliği ayarla
                btnBaglan.Enabled = true;
                btnKES.Enabled = false; // İsim DÜZELTİLDİ: btnKES
                
                MessageBox.Show("Port Kapatıldı.");
            }
        }
        
        // --- 3. SAAT GÖNDER BUTONU ---
        void BtnSaatGonderClick(object sender, EventArgs e)
        {
            if(serialPort1.IsOpen)
            {
                // Format: S12:30:00#
                string veri = "S" + txtSaat.Text + "#";
                serialPort1.Write(veri);
            }
            else
            {
                MessageBox.Show("Lütfen önce 'Port Aç' butonuna basınız.");
            }
        }
        
        // --- 4. METİN (MESAJ) GÖNDER BUTONU ---
        void BtnMetinGonderClick(object sender, EventArgs e)
        {
            if(serialPort1.IsOpen)
            {
                string metin = txtMetin.Text;
                
                // Format: MMerhaba#
                string veri = "M" + metin + "#";
                serialPort1.Write(veri);
            }
            else
            {
                MessageBox.Show("Lütfen önce 'Port Aç' butonuna basınız.");
            }
        }
    }
}