using Microsoft.Win32;
using System.Diagnostics;

namespace Launcher
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            Text = "GardenGate Launcher";
            Icon = Icon.ExtractAssociatedIcon(Application.ExecutablePath);
            LoadConfig();
            ApplyTheme(IsDarkMode());
        }
        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            SaveConfig();
            base.OnFormClosing(e);
        }

        private void LoadModData(string gameExePath)
        {
            ModDataComboBox.Items.Clear();

            string gameDir = Path.GetDirectoryName(gameExePath)!;
            string modDataDir = Path.Combine(gameDir, "ModData");

            if (!Directory.Exists(modDataDir))
                return;

            foreach (string dir in Directory.GetDirectories(modDataDir))
            {
                ModDataComboBox.Items.Add(Path.GetFileName(dir));
            }
            if (ModDataComboBox.Items.Count > 0) { 
                ModDataComboBox.SelectedIndex = 0; 
            }
        }

        private bool IsDarkMode()
        {
            string? gtkTheme = Environment.GetEnvironmentVariable("GTK_THEME");

            if (!string.IsNullOrEmpty(gtkTheme))
            {
                string lower = gtkTheme.ToLowerInvariant();

                if (lower.Contains(":dark"))
                    return true;

                if (lower.Contains(":light"))
                    return false;
            }

            using var key = Registry.CurrentUser.OpenSubKey(
                @"Software\Microsoft\Windows\CurrentVersion\Themes\Personalize");

            object? value = key?.GetValue("AppsUseLightTheme");
            return value is int i && i == 0;
        }
        private void ApplyTheme(bool dark)
        {
            Color back = dark ? Color.FromArgb(32, 32, 32) : SystemColors.Control;
            Color fore = dark ? Color.White : SystemColors.ControlText;
            Color inputBack = dark ? Color.FromArgb(45, 45, 45) : Color.White;
            Color disabledBack = dark ? Color.FromArgb(60, 60, 60) : SystemColors.ControlLight;

            this.BackColor = back;

            foreach (Control c in Controls)
            {
                c.ForeColor = fore;
                switch (c)
                {
                    case TextBox or ComboBox:
                        c.BackColor = c.Enabled ? inputBack : disabledBack;
                        break;

                    case Button:
                        c.BackColor = dark ? Color.FromArgb(55, 55, 55) : SystemColors.Control;
                        break;

                    case Label:
                        c.BackColor = back;
                        break;
                }
            }
        }

        private void ModDataCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            ModDataComboBox.Enabled = ModDataCheckBox.Checked;
        }
        private void ModDataComboBox_DrawItem(object sender, DrawItemEventArgs e)
        {
            if (e.Index < 0) return;

            Color fore = IsDarkMode() ? Color.White : Color.Black;

            string text = ModDataComboBox.Items[e.Index]?.ToString() ?? string.Empty;
            Font font = e.Font ?? this.Font;

            e.Graphics.DrawString(text, font, new SolidBrush(fore), e.Bounds);
        }

        private void SaveConfig()
        {
            var lines = File.Exists("config.txt")
                ? File.ReadAllLines("config.txt").ToList()
                : new List<string>();

            while (lines.Count < 7)
                lines.Add("");

            lines[0] = UsernameTextBox.Text;
            lines[1] = ServerIpTextBox.Text;
            lines[3] = ModDataCheckBox.Checked.ToString();
            lines[4] = ModDataComboBox.SelectedItem?.ToString() ?? "";
            lines[5] = ArgsTextBox.Text;

            bool isGW2 = GW2Radio.Checked;
            if (isGW2)
                lines[6] = GamePathTextBox.Text;   
            else
                lines[2] = GamePathTextBox.Text;   

            File.WriteAllLines("config.txt", lines);
        }

        private void LoadConfig()
        {
            if (!File.Exists("config.txt"))
                return;

            string[] lines = File.ReadAllLines("config.txt");

            if (lines.Length > 0) UsernameTextBox.Text = lines[0];
            if (lines.Length > 1) ServerIpTextBox.Text = lines[1];

            string savedModPack = lines.Length > 4 ? lines[4] : "";

            if (lines.Length > 3 && bool.TryParse(lines[3], out bool modEnabled))
                ModDataCheckBox.Checked = modEnabled;

            if (lines.Length > 5)
                ArgsTextBox.Text = lines[5];

            bool isGW2 = GW2Radio.Checked;

            GamePathTextBox.Text =
                isGW2 && lines.Length > 6 ? lines[6] :
                !isGW2 && lines.Length > 2 ? lines[2] :
                "";

            if (File.Exists(GamePathTextBox.Text))
            {
                LoadModData(GamePathTextBox.Text);

                if (!string.IsNullOrWhiteSpace(savedModPack) && ModDataComboBox.Items.Contains(savedModPack))
                    ModDataComboBox.SelectedItem = savedModPack;
            }
        }

        private void BrowseGameButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();

            bool isGW2 = GW2Radio.Checked;


            dlg.Filter = isGW2
                ? "PVZGW2|GW2.Main_Win64_Retail.exe"
                : "PVZGW1|PVZ.Main_Win64_Retail.exe";

            if (dlg.ShowDialog() != DialogResult.OK)
                return;

            GamePathTextBox.Text = dlg.FileName;

            var lines = File.Exists("config.txt")
                ? File.ReadAllLines("config.txt").ToList()
                : Enumerable.Repeat("", 7).ToList();

            while (lines.Count < 7)
                lines.Add("");

            lines[isGW2 ? 6 : 2] = dlg.FileName;

            File.WriteAllLines("config.txt", lines);
        }

        private void GameRadioChanged(object sender, EventArgs e)
        {
            if (!((RadioButton)sender).Checked)
                return;

            if (!File.Exists("config.txt"))
            {
                GamePathTextBox.Text = "";
                ModDataComboBox.Items.Clear();
                return;
            }

            var lines = File.ReadAllLines("config.txt");
            bool isGW2 = GW2Radio.Checked;

            GamePathTextBox.Text =
                isGW2 && lines.Length > 6 ? lines[6] :
                !isGW2 && lines.Length > 2 ? lines[2] :
                "";

            ModDataComboBox.Items.Clear();
            if (File.Exists(GamePathTextBox.Text))
                LoadModData(GamePathTextBox.Text);
        }
        private void LaunchButton_Click(object sender, EventArgs e)
        {
            if (!File.Exists(GamePathTextBox.Text))
            {
                MessageBox.Show("Invalid game path.");
                return;
            }

            List<string> argList = new();

            if (!string.IsNullOrWhiteSpace(ArgsTextBox.Text))
                argList.Add(ArgsTextBox.Text.Trim());

            if (!string.IsNullOrWhiteSpace(UsernameTextBox.Text))
                argList.Add($"-name {UsernameTextBox.Text}");

            if (!string.IsNullOrWhiteSpace(ServerIpTextBox.Text))
                argList.Add($"-Client.ServerIp {ServerIpTextBox.Text}");

            if (!string.IsNullOrWhiteSpace(ServerPasswordTextBox.Text))
                argList.Add($"-Server.ServerPassword {ServerPasswordTextBox.Text}");

            if (ModDataCheckBox.Checked && ModDataComboBox.SelectedItem != null)
                argList.Add($"-dataPath ModData/{ModDataComboBox.SelectedItem}");

            string Args = string.Join(" ", argList);
            SaveConfig();

            Process.Start(new ProcessStartInfo
            {
                FileName = GamePathTextBox.Text,
                Arguments = Args,
                WorkingDirectory = Path.GetDirectoryName(GamePathTextBox.Text)
            });
        }
    }
}

