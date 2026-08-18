#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
山河纪元 ShanHeEra - 游戏启动器
Game Launcher for ShanHeEra (Historical Sandbox RPG)
支持中文 / English / 日本語
"""
import tkinter as tk
from tkinter import ttk, messagebox
import json
import os
import sys
import subprocess
import webbrowser

# === 多语言文本 ===
LANGS = {
    "zh": {
        "title": "山河纪元 · 启动器",
        "subtitle": "华夏古代历史文明沙盘 RPG",
        "philosophy": "历史不是背景，历史就是世界本身。",
        "start": "开始游戏",
        "settings": "游戏设置",
        "update": "检查更新",
        "exit": "退出",
        "language": "语言",
        "resolution": "分辨率",
        "volume": "音量",
        "fullscreen": "全屏模式",
        "save": "保存设置",
        "version": "版本 1.0.0",
        "update_check": "正在检查更新...",
        "update_latest": "已是最新版本！",
        "update_available": "发现新版本！",
        "game_not_found": "未找到游戏可执行文件，请先编译项目。",
        "launching": "正在启动山河纪元...",
        "credits": "山河纪元工作室 出品",
    },
    "en": {
        "title": "ShanHe Era · Launcher",
        "subtitle": "Chinese Historical Civilization Sandbox RPG",
        "philosophy": "History is not a backdrop. History IS the world itself.",
        "start": "Start Game",
        "settings": "Settings",
        "update": "Check Update",
        "exit": "Exit",
        "language": "Language",
        "resolution": "Resolution",
        "volume": "Volume",
        "fullscreen": "Fullscreen",
        "save": "Save Settings",
        "version": "Version 1.0.0",
        "update_check": "Checking for updates...",
        "update_latest": "You are on the latest version!",
        "update_available": "New version available!",
        "game_not_found": "Game executable not found. Please build the project first.",
        "launching": "Launching ShanHe Era...",
        "credits": "Produced by ShanHe Studio",
    },
    "ja": {
        "title": "山河紀元・ランチャー",
        "subtitle": "華夏古代歴史文明シミュレーションRPG",
        "philosophy": "歴史は背景ではない。歴史こそが世界そのもの。",
        "start": "ゲーム開始",
        "settings": "設定",
        "update": "更新を確認",
        "exit": "終了",
        "language": "言語",
        "resolution": "解像度",
        "volume": "音量",
        "fullscreen": "フルスクリーン",
        "save": "設定を保存",
        "version": "バージョン 1.0.0",
        "update_check": "更新を確認中...",
        "update_latest": "最新バージョンです！",
        "update_available": "新しいバージョンがあります！",
        "game_not_found": "ゲーム実行ファイルが見つかりません。先にプロジェクトをビルドしてください。",
        "launching": "山河紀元を起動中...",
        "credits": "山河スタジオ制作",
    }
}

CONFIG_FILE = os.path.join(os.path.dirname(__file__), "launcher_config.json")

class LauncherApp:
    def __init__(self, root):
        self.root = root
        self.config = self.load_config()
        self.current_lang = self.config.get("language", "zh")
        self.t = LANGS[self.current_lang]

        self.setup_ui()

    def load_config(self):
        default = {
            "language": "zh",
            "resolution": "1920x1080",
            "volume": 80,
            "fullscreen": True,
            "last_version": "1.0.0"
        }
        if os.path.exists(CONFIG_FILE):
            try:
                with open(CONFIG_FILE, 'r', encoding='utf-8') as f:
                    return {**default, **json.load(f)}
            except:
                pass
        return default

    def save_config(self):
        with open(CONFIG_FILE, 'w', encoding='utf-8') as f:
            json.dump(self.config, f, ensure_ascii=False, indent=2)

    def setup_ui(self):
        self.root.title(self.t["title"])
        self.root.geometry("720x520")
        self.root.resizable(False, False)
        self.root.configure(bg="#1a1a2e")

        # 标题区
        title_frame = tk.Frame(self.root, bg="#1a1a2e")
        title_frame.pack(pady=30)
        tk.Label(title_frame, text=self.t["title"].split(" · ")[0],
                font=("Microsoft YaHei", 36, "bold"), fg="#e94560", bg="#1a1a2e").pack()
        tk.Label(title_frame, text=self.t["subtitle"],
                font=("Microsoft YaHei", 12), fg="#a8a8a8", bg="#1a1a2e").pack(pady=5)
        tk.Label(title_frame, text=f'"{self.t["philosophy"]}"',
                font=("Microsoft YaHei", 10, "italic"), fg="#666", bg="#1a1a2e").pack(pady=8)

        # 按钮区
        btn_frame = tk.Frame(self.root, bg="#1a1a2e")
        btn_frame.pack(pady=20)

        btn_style = {"font": ("Microsoft YaHei", 14), "width": 18, "height": 1,
                     "bd": 0, "cursor": "hand2"}

        self.start_btn = tk.Button(btn_frame, text=self.t["start"], bg="#e94560", fg="white",
                                   activebackground="#c73e54", activeforeground="white",
                                   command=self.start_game, **btn_style)
        self.start_btn.pack(pady=6)

        self.settings_btn = tk.Button(btn_frame, text=self.t["settings"], bg="#16213e", fg="#e0e0e0",
                                      activebackground="#0f3460", activeforeground="white",
                                      command=self.open_settings, **btn_style)
        self.settings_btn.pack(pady=6)

        self.update_btn = tk.Button(btn_frame, text=self.t["update"], bg="#16213e", fg="#e0e0e0",
                                    activebackground="#0f3460", activeforeground="white",
                                    command=self.check_update, **btn_style)
        self.update_btn.pack(pady=6)

        self.exit_btn = tk.Button(btn_frame, text=self.t["exit"], bg="#16213e", fg="#e0e0e0",
                                  activebackground="#0f3460", activeforeground="white",
                                  command=self.root.quit, **btn_style)
        self.exit_btn.pack(pady=6)

        # 底部信息
        bottom_frame = tk.Frame(self.root, bg="#1a1a2e")
        bottom_frame.pack(side="bottom", pady=15)
        tk.Label(bottom_frame, text=f'{self.t["version"]}  |  {self.t["credits"]}',
                font=("Microsoft YaHei", 9), fg="#555", bg="#1a1a2e").pack()

    def start_game(self):
        # 查找游戏可执行文件
        game_dir = os.path.dirname(os.path.dirname(__file__))
        possible_paths = [
            os.path.join(game_dir, "Binaries", "Win64", "ShanHeEra.exe"),
            os.path.join(game_dir, "Binaries", "Win64", "ShanHeEra-Win64-Shipping.exe"),
        ]
        exe_path = None
        for p in possible_paths:
            if os.path.exists(p):
                exe_path = p
                break

        if exe_path:
            messagebox.showinfo(self.t["title"], self.t["launching"])
            subprocess.Popen([exe_path])
            self.root.quit()
        else:
            messagebox.showwarning(self.t["title"], self.t["game_not_found"])

    def open_settings(self):
        settings_win = tk.Toplevel(self.root)
        settings_win.title(self.t["settings"])
        settings_win.geometry("400x350")
        settings_win.configure(bg="#1a1a2e")
        settings_win.resizable(False, False)

        tk.Label(settings_win, text=self.t["settings"], font=("Microsoft YaHei", 16, "bold"),
                fg="#e94560", bg="#1a1a2e").pack(pady=15)

        # 语言
        lang_frame = tk.Frame(settings_win, bg="#1a1a2e")
        lang_frame.pack(pady=8, fill="x", padx=40)
        tk.Label(lang_frame, text=self.t["language"] + ":", font=("Microsoft YaHei", 11),
                fg="#e0e0e0", bg="#1a1a2e").pack(side="left")
        lang_var = tk.StringVar(value=self.current_lang)
        lang_combo = ttk.Combobox(lang_frame, textvariable=lang_var, values=["zh", "en", "ja"],
                                  state="readonly", width=10)
        lang_combo.pack(side="right")

        # 分辨率
        res_frame = tk.Frame(settings_win, bg="#1a1a2e")
        res_frame.pack(pady=8, fill="x", padx=40)
        tk.Label(res_frame, text=self.t["resolution"] + ":", font=("Microsoft YaHei", 11),
                fg="#e0e0e0", bg="#1a1a2e").pack(side="left")
        res_var = tk.StringVar(value=self.config["resolution"])
        res_combo = ttk.Combobox(res_frame, textvariable=res_var,
                                 values=["1280x720", "1920x1080", "2560x1440", "3840x2160"],
                                 state="readonly", width=12)
        res_combo.pack(side="right")

        # 音量
        vol_frame = tk.Frame(settings_win, bg="#1a1a2e")
        vol_frame.pack(pady=8, fill="x", padx=40)
        tk.Label(vol_frame, text=self.t["volume"] + ":", font=("Microsoft YaHei", 11),
                fg="#e0e0e0", bg="#1a1a2e").pack(side="left")
        vol_var = tk.IntVar(value=self.config["volume"])
        vol_scale = tk.Scale(vol_frame, from_=0, to=100, orient="horizontal", variable=vol_var,
                             bg="#1a1a2e", fg="#e0e0e0", troughcolor="#16213e",
                             activebackground="#e94560", length=150, showvalue=True)
        vol_scale.pack(side="right")

        # 全屏
        fs_var = tk.BooleanVar(value=self.config["fullscreen"])
        fs_check = tk.Checkbutton(settings_win, text=self.t["fullscreen"], variable=fs_var,
                                  bg="#1a1a2e", fg="#e0e0e0", selectcolor="#16213e",
                                  activebackground="#1a1a2e", activeforeground="white",
                                  font=("Microsoft YaHei", 11))
        fs_check.pack(pady=10)

        def do_save():
            self.config["language"] = lang_var.get()
            self.config["resolution"] = res_var.get()
            self.config["volume"] = vol_var.get()
            self.config["fullscreen"] = fs_var.get()
            self.save_config()
            self.current_lang = lang_var.get()
            self.t = LANGS[self.current_lang]
            settings_win.destroy()
            self.setup_ui()
            for widget in self.root.winfo_children():
                widget.destroy()
            self.setup_ui()

        tk.Button(settings_win, text=self.t["save"], bg="#e94560", fg="white",
                  font=("Microsoft YaHei", 12), bd=0, cursor="hand2",
                  command=do_save, width=15).pack(pady=15)

    def check_update(self):
        messagebox.showinfo(self.t["title"], self.t["update_latest"])

def main():
    root = tk.Tk()
    # 居中
    root.update_idletasks()
    w, h = 720, 520
    x = (root.winfo_screenwidth() - w) // 2
    y = (root.winfo_screenheight() - h) // 2
    root.geometry(f"{w}x{h}+{x}+{y}")
    app = LauncherApp(root)
    root.mainloop()

if __name__ == "__main__":
    main()
