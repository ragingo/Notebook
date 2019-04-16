
cls

Set-Variable -Name ExportFileNameFormat -Option Constant -Value 'd:/logs/{0}_{1:yyyyMMddHHmmss}.evtx'
$targets = @("Application", "Security", "System")
$logs = 
	Get-WmiObject -class Win32_NTEventLogFile | 
		where { 
			$targets -contains $_.LogFilename
		} 

foreach($log in $logs) {
	echo $log.Name
    $log.PSBase.Scope.Options.EnablePrivileges = $true
    [void]$log.BackupEventLog(($ExportFileNameFormat -f $log.LogFilename, (Get-Date)))
}