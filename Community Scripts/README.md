#Scripts

This is a collection of Powershell scripts, written during the whole driver development. PR's are welcome for improvements or other/more functions. There haven't been a focus on keeping the scripts "up to date" all the time. Which means changes that might have occurred in the modules that these scripts depend upon, haven't been corrected either. These are more ment as a framework for "tinkers" to make use of Powershell to change settings on the fly. To use these, you should be at least be comfortable with Powershell and understand Admin privilegiets, risk/reward.

Anyway here is a short description of each script:

silent‑install.ps1 – Silently fetches the latest signed Virtual Display Driver from GitHub and installs it via DevCon, then tidies up the temp workspace.
modules_install.bat – Opens an elevated PowerShell session that installs the DisplayConfig and MonitorConfig modules so every helper script has its prerequisites.
set‑dependencies.ps1 – Verifies the exact module versions required, installs or imports them on demand, and aborts downstream execution if anything is missing.
get_disp_num.ps1 – Returns the numeric adapter ID of the VDD screen by scanning WMI for your custom “MTT1337” monitor identifier.
changeres‑VDD.ps1 – Hot‑swaps the virtual panel’s resolution to the width and height you pass on the command line.
refreshrate‑VDD.ps1 – Changes the VDD monitor’s refresh rate (30‑‑500 Hz) after validating the value against a safe list.
rotate‑VDD.ps1 – Rotates the virtual display 90°, 180°, or 270° by calling DisplayConfig with the matching rotation token.
scale‑VDD.ps1 – Sets or resets DPI scaling on the virtual monitor, respecting Windows’ maximum allowed scale factor.
HDRswitch‑VDD.ps1 – Flips the virtual screen between SDR (8‑bit) and HDR (10‑bit) colour modes in one click. Unsure if it works anylonger, since changes in moduels)
primary‑VDD.ps1 – Makes the virtual display the Windows primary so you can stream or remote‑desktop from a headless rig.
toggle‑VDD.ps1 – A one‑click PowerShell switch that first elevates itself to admin, then enables or disables your Virtual Display Driver and immediately flips Windows between Extended and Cloned desktops, perfect for streamers who need to bring a virtual monitor online or offline on demand.
winp‑VDD.ps1 – A lightweight companion script that leaves the driver untouched and simply yo‑yos Windows between Extend and Clone modes, giving you an instant “presentation toggle” when the virtual display should stay permanently enabled.


#脚本  这是在整个驱动程序开发过程中编写的 Powershell 脚本集合。**欢迎提交 PR 以改进或添加其他/更多功能。**我们并没有专注于始终保持脚本的“最新”。**这意味着这些脚本所依赖的模块中可能发生的更改也尚未得到纠正。**这些脚本更像是一个框架，供“修补匠”使用 Powershell 动态更改设置。**要使用这些脚本，您至少应该熟悉 Powershell 并了解管理员权限、风险/回报。**  以下是每个脚本的简短描述：

**silent‑install.ps1 – 静默地从 GitHub 获取最新签名的 VirtualDisplayDriver，并通过 DevCon 安装，然后整理临时工作区。** modules\_install.bat – 打开一个提升权限的 PowerShell 会话，安装 DisplayConfig 和 MonitorConfig 模块，因此每个辅助脚本都有其先决条件。

**set‑dependencies.ps1 – 验证所需的确切模块版本，按需安装或导入，并在缺少任何内容时中止下游执行。**

get\_disp\_num.ps1 – 通过扫描 WMI 中的自定义“MTT1337”显示器标识符，返回 VDD 屏幕的数字适配器 ID。changeres‑VDD.ps1 – 将虚拟面板的分辨率热切换为您在命令行中传递的宽度和高度。

refreshrate‑VDD.ps1 – 在根据安全列表验证值后，更改 VDD 显示器的刷新率 (30-500Hz)。

rotate‑VDD.ps1 – 通过使用匹配的旋转令牌调用 DisplayConfig，将虚拟显示器旋转 90°、180° 或 270°。

scale‑VDD.ps1 – 设置或重置虚拟显示器的 DPI 缩放比例，并遵循 Windows 允许的最大缩放比例。

HDRswitch‑VDD.ps1 – 一键切换虚拟屏幕在 SDR（8 位）和 HDR（10 位）色彩模式之间切换。**由于模块变更，不确定该功能是否还能继续使用。 

**primary‑VDD.ps1 – 将虚拟显示器设置为 Windows 主显示器，以便您可以从无头设备进行串流或远程桌面控制。** toggle‑VDD.ps1 – 一键式 PowerShell 开关，首先提升至管理员权限，然后启用或禁用 VirtualDisplayDriver，并立即在扩展桌面和克隆桌面之间切换 Windows，非常适合需要按需切换虚拟显示器的串流者。

winp‑VDD.ps1 – 轻量级配套脚本，不影响驱动程序，只需在扩展模式和克隆模式之间切换 Windows，即可在虚拟显示器应保持永久启用状态时，提供即时“演示切换”。
