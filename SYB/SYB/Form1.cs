using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Windows.Forms;
// ReSharper disable InconsistentNaming

namespace SYB
{
    public partial class Form1 : Form
    {
        private readonly RawViewForm rawViewForm;
        private string _tx2alPath;
        private string _serSendPath;
        private string _comPort = "COM4";

        public Form1()
        {
            InitializeComponent();
            rawViewForm = new RawViewForm();
            rawViewForm.Log("SYB V1.0 Online\n");
            rawViewForm.Log("To change COM port type its name & click Send. Port is currently " + _comPort);

            var rootPath = Path.GetDirectoryName(System.Reflection.Assembly.GetEntryAssembly().Location) ?? "";

            _tx2alPath = Path.Combine(rootPath, "tx2al.exe");
            _serSendPath = Path.Combine(rootPath, "sersend.exe");

            buttonSend.Enabled = File.Exists(_tx2alPath) && File.Exists(_serSendPath);

            rawViewForm.Log(File.Exists(_tx2alPath) ? "tx2al is available" : "Error: tx2al.exe not found");
            rawViewForm.Log(File.Exists(_serSendPath) ? "sersend is available" : "Error: sersend.exe not found");
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            e.KeyChar = char.ToUpperInvariant(e.KeyChar);
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            var englishText = textBox1.Text.Trim();
            if (englishText.Length == 4 && englishText.StartsWith("COM"))
            {
                _comPort = englishText;
                rawViewForm.Log("COM port is now " + _comPort);
                return;
            }

            englishText = englishText.Replace("\r\n", " ");
            if (string.IsNullOrEmpty(englishText)) return;

            var tempFile = Path.GetTempFileName();

            var allophones = ConvertTextToAllophones(englishText, tempFile);
            rawViewForm.Log(englishText);
            rawViewForm.Log(allophones);

            SendDataToSerial(allophones, tempFile);

            File.Delete(tempFile);
        }

        private void SendDataToSerial(byte[] convertedData, string destFile)
        {
            if (convertedData.Length == 0) return;

            var startInfoe = new ProcessStartInfo()
            {
                CreateNoWindow = true,
                FileName = _serSendPath,
                UseShellExecute = false,
                Arguments = destFile + " " + _comPort,
                RedirectStandardOutput = true
            };

            var p = new Process() {
                StartInfo = startInfoe
            };

            try
            {
                p.Start();
                var output = p.StandardOutput.ReadToEnd();
                p.WaitForExit();

                if (p.ExitCode == 0) return;

                var error = output.Split('\n').Select(l => "   " + l).Aggregate((current, next) => current + "\n" + next);
                
                rawViewForm.Log("Error sending serial:");
                rawViewForm.Log(error);
            }
            catch (Exception ex)
            {
                rawViewForm.Log("Exception sending serial: " + ex.Message);
            }
        }

        private byte[] ConvertTextToAllophones(string text, string destFile)
        {
            var allophoneData = new byte[0];

            var startInfoe = new ProcessStartInfo()
            {
                CreateNoWindow = true,
                FileName = _tx2alPath,
                UseShellExecute = false,
                Arguments = "-T \"" + text + "\" -O " + destFile
            };

            var p = new Process()
            {
                StartInfo = startInfoe
            };

            try
            {
                p.Start();
                p.WaitForExit();

                if (p.ExitCode == 0 && File.Exists(destFile))
                {
                    allophoneData = File.ReadAllBytes(destFile);
                }
            }
            catch (Exception ex)
            {
                rawViewForm.Log("Exception converting text: " + ex.Message);
            }

            return allophoneData;
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked) rawViewForm.Show();
            else rawViewForm.Hide();
        }
    }
}
