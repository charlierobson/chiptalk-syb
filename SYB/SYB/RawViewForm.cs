using System;
using System.Text;
using System.Windows.Forms;

namespace SYB
{
    public partial class RawViewForm : Form
    {
        private readonly StringBuilder _sb;

        public RawViewForm()
        {
            InitializeComponent();
            _sb = new StringBuilder();
        }

        public void Log(string message)
        {
            _sb.AppendLine(message);
            textBox1.Text = _sb.ToString();
        }

        public void Log(byte[] bytes)
        {
            _sb.AppendLine(BitConverter.ToString(bytes).Replace("-", " "));
            textBox1.Text = _sb.ToString();
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            e.Handled = true;
        }
    }
}
