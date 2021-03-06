
#シェル名 "2010/08/21" "2010/08/22"
#Set-ExecutionPolicy RemoteSigned    ※Windows上初回ps1ファイル実行時に必要（規定値では実行許可されてない）
#http://blogs.wankuma.com/mutaguchi/

cls
Set-Variable -Name using -Option ReadOnly -Value (@{ 
	reader = "System.Diagnostics.Eventing.Reader.{0}"
})

Set-Variable -Name EXPORT_DRIVE -Value "d:/" -Option Constant
Set-Variable -Name EXPORT_FILENAME_FORMAT -Value ($EXPORT_DRIVE + "logs/eventlog_{0:yyyyMMddHHmmss}.csv") -Option Constant

if($Args.Length -ne 2) {
    return
}

[ref]$from = Get-Date
[ref]$to = Get-Date

# 入力チェック
if(!([DateTime]::TryParse($Args[0], $from))) {
    return
}
if(!([DateTime]::TryParse($Args[1], $to))) {
    return
}
if($from.Value -gt $to.Value) {
    return
}

echo $from.Value
echo $to.Value

# 保存先
$filename = $EXPORT_FILENAME_FORMAT -f (Get-Date)
Echo $filename

# 日付範囲指定でフィルタリング
Echo "filtering..."
$events = 
    Get-WinEvent -LogName Security | 
        where {
            ($_.TimeCreated -ge $from.Value) -and 
            ($_.TimeCreated -lt $to.Value)
        } | 
        select `
            Id,
            Version,
            Level,
            Task,
            OpCode,
            RecordId,
            ProviderId,
            ProviderName,
            LogName,
            ProcessId,
            ThreadId,
            MachineName,
            UserId,
            TimeCreated,
            LevelDisplayName,
            OpCodeDisplayName,
            TaskDisplayName,
			Properties

Echo "Exporting..."
$events | Export-Csv $filename –Encoding UTF8

Echo "Complete!"




#Get-WmiObject -Query "select * from win32_ntlogevent where logfile = 'Security'"

#Get-WmiObject -Class Win32_NTLogEvent -Filter "logfile = 'Security'" | 
#    Format-Table -Property CategoryString, EventCode, Message

<# evtxを直にバックアップ（単にファイルコピーしてるみたい）
Echo "file backup..."

$targets = "Application", "Security", "System"
$logs = Get-WmiObject -class Win32_NTEventLogFile
$count = 0

foreach($log in $logs) {
    if(-not($targets -contains $log.LogFilename)) {
        continue
    }
    $count++
    
    "{0,3:###}%`t{1}" -f ($count / $targets.Length * 100), $log.Name

    $log.PSBase.Scope.Options.EnablePrivileges = $true
    [void]$log.BackupEventLog("d:/logs/{0}.evtx" -f $log.LogFilename)
}

Echo "complete!"#>