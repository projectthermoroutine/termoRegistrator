﻿//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.42000
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

namespace Registrator.Properties {
    
    
    [global::System.Runtime.CompilerServices.CompilerGeneratedAttribute()]
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.VisualStudio.Editors.SettingsDesigner.SettingsSingleFileGenerator", "15.6.0.0")]
    internal sealed partial class Settings : global::System.Configuration.ApplicationSettingsBase {
        
        private static Settings defaultInstance = ((Settings)(global::System.Configuration.ApplicationSettingsBase.Synchronized(new Settings())));
        
        public static Settings Default {
            get {
                return defaultInstance;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("./Films")]
        public string lastIRBDir {
            get {
                return ((string)(this["lastIRBDir"]));
            }
            set {
                this["lastIRBDir"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("Projects")]
        public string lastProjDir {
            get {
                return ((string)(this["lastProjDir"]));
            }
            set {
                this["lastProjDir"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("./")]
        public string lastDir {
            get {
                return ((string)(this["lastDir"]));
            }
            set {
                this["lastDir"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("Projects")]
        public string projectsDir {
            get {
                return ((string)(this["projectsDir"]));
            }
            set {
                this["projectsDir"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("./Films")]
        public string irbDir {
            get {
                return ((string)(this["irbDir"]));
            }
            set {
                this["irbDir"] = value;
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("False")]
        public bool irbInProjectFolder {
            get {
                return ((bool)(this["irbInProjectFolder"]));
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("False")]
        public bool rzInProjectFolder {
            get {
                return ((bool)(this["rzInProjectFolder"]));
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("./")]
        public string dbPath {
            get {
                return ((string)(this["dbPath"]));
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("./")]
        public string dbBlobsPath {
            get {
                return ((string)(this["dbBlobsPath"]));
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("True")]
        public bool dbBlobsInDbPath {
            get {
                return ((bool)(this["dbBlobsInDbPath"]));
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("False")]
        public bool grabberCamMode {
            get {
                return ((bool)(this["grabberCamMode"]));
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("127.0.0.1")]
        public string pd_ip {
            get {
                return ((string)(this["pd_ip"]));
            }
            set {
                this["pd_ip"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("32222")]
        public ushort pd_port {
            get {
                return ((ushort)(this["pd_port"]));
            }
            set {
                this["pd_port"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("127.0.0.1")]
        public string pd_events_ip {
            get {
                return ((string)(this["pd_events_ip"]));
            }
            set {
                this["pd_events_ip"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("32223")]
        public ushort pd_events_port {
            get {
                return ((ushort)(this["pd_events_port"]));
            }
            set {
                this["pd_events_port"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("300000")]
        public long TrackHalfVeiwSector {
            get {
                return ((long)(this["TrackHalfVeiwSector"]));
            }
            set {
                this["TrackHalfVeiwSector"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("5")]
        public byte synchronizer_counter_size {
            get {
                return ((byte)(this["synchronizer_counter_size"]));
            }
            set {
                this["synchronizer_counter_size"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("127.0.0.1")]
        public string pd_i_ip {
            get {
                return ((string)(this["pd_i_ip"]));
            }
            set {
                this["pd_i_ip"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("127.0.0.1")]
        public string pd_i_events_ip {
            get {
                return ((string)(this["pd_i_events_ip"]));
            }
            set {
                this["pd_i_events_ip"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("600")]
        public ushort maxFramesInFile {
            get {
                return ((ushort)(this["maxFramesInFile"]));
            }
            set {
                this["maxFramesInFile"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("0")]
        public long camera_offset {
            get {
                return ((long)(this["camera_offset"]));
            }
            set {
                this["camera_offset"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("0")]
        public long current_camera_offset {
            get {
                return ((long)(this["current_camera_offset"]));
            }
            set {
                this["current_camera_offset"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("Logs")]
        public string logs_dir {
            get {
                return ((string)(this["logs_dir"]));
            }
            set {
                this["logs_dir"] = value;
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("registrator.config.xml")]
        public string log_config_data {
            get {
                return ((string)(this["log_config_data"]));
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("False")]
        public bool debug_info {
            get {
                return ((bool)(this["debug_info"]));
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("10000")]
        public int DefaultPicketLength {
            get {
                return ((int)(this["DefaultPicketLength"]));
            }
            set {
                this["DefaultPicketLength"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("00000000-0000-0000-0000-000000000000")]
        public global::System.Guid Setting {
            get {
                return ((global::System.Guid)(this["Setting"]));
            }
            set {
                this["Setting"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("#FF008000")]
        public global::System.Windows.Media.Color TrackPanel_BackGroungColor {
            get {
                return ((global::System.Windows.Media.Color)(this["TrackPanel_BackGroungColor"]));
            }
            set {
                this["TrackPanel_BackGroungColor"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("True")]
        public bool TrackPanel_VisibleEquipment {
            get {
                return ((bool)(this["TrackPanel_VisibleEquipment"]));
            }
            set {
                this["TrackPanel_VisibleEquipment"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("#FF008000")]
        public global::System.Windows.Media.Color TrackPanel_TrailMarkerColor {
            get {
                return ((global::System.Windows.Media.Color)(this["TrackPanel_TrailMarkerColor"]));
            }
            set {
                this["TrackPanel_TrailMarkerColor"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("IRBFrameShots")]
        public string IRBFrameShotSavePath {
            get {
                return ((string)(this["IRBFrameShotSavePath"]));
            }
            set {
                this["IRBFrameShotSavePath"] = value;
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("False")]
        public bool enableBadPixelsControl {
            get {
                return ((bool)(this["enableBadPixelsControl"]));
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("<bad_pixels_settings><camera_settings SN = \"1209017\" description = \"Baku teploviz" +
            "or\" shot_width = \"640\" shot_height = \"480\"><pixels good_pixel_offset=\"1\"><pixel " +
            "x=\"223\" y=\"25\" /></pixels></camera_settings></bad_pixels_settings>")]
        public string BadPixelsSettings {
            get {
                return ((string)(this["BadPixelsSettings"]));
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute(":autofocus")]
        public string auto_focus_cmd_str {
            get {
                return ((string)(this["auto_focus_cmd_str"]));
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute(":autonuc")]
        public string nuc_cmd_str {
            get {
                return ((string)(this["nuc_cmd_str"]));
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("AlarmTermogramms")]
        public string RuntimeAlarmFramesPath {
            get {
                return ((string)(this["RuntimeAlarmFramesPath"]));
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("1")]
        public float CorrectionTemperatureFactor {
            get {
                return ((float)(this["CorrectionTemperatureFactor"]));
            }
            set {
                this["CorrectionTemperatureFactor"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("0")]
        public float CorrectionTemperatureOffset {
            get {
                return ((float)(this["CorrectionTemperatureOffset"]));
            }
            set {
                this["CorrectionTemperatureOffset"] = value;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("False")]
        public bool UseCorrectionTemperature {
            get {
                return ((bool)(this["UseCorrectionTemperature"]));
            }
            set {
                this["UseCorrectionTemperature"] = value;
            }
        }
        
        [global::System.Configuration.ApplicationScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.SpecialSettingAttribute(global::System.Configuration.SpecialSetting.ConnectionString)]
        [global::System.Configuration.DefaultSettingValueAttribute("Data source=(LocalDB)\\MSSQLLocalDB;attachdbfilename=|DataDirectory|\\DB\\NEW_SCHEMA" +
            "\\METROCARD_NEW.mdf;Integrated Security=True;Connect Timeout=30")]
        public string MetrocardConnectionString {
            get {
                return ((string)(this["MetrocardConnectionString"]));
            }
        }
    }
}
