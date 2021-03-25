# https://github.com/PowerShell/PowerShell#get-powershell

# 参考: https://superuser.com/a/844034

$BUILD_DIR = $Args[0]
$BUILD_TEMP_FILE = "$BUILD_DIR/build/.building"

$watcher = New-Object IO.FileSystemWatcher "$BUILD_DIR", "*.swift"
$watcher.NotifyFilter = [IO.NotifyFilters]'LastWrite'
$watcher.IncludeSubdirectories = $true

$action = {
    $e = $Event.SourceEventArgs
    $path = $e.FullPath
    $name = $e.Name
    $changeType = $e.ChangeType
    $now = [DateTime]::Now.ToString("yyyy-MM-dd HH:mm:ss")
    echo "$now [watcher] [$changeType] $path"

    touch "$BUILD_TEMP_FILE"
    make
    rm "$BUILD_TEMP_FILE"
}

$job1 = Register-ObjectEvent -InputObject $watcher -EventName Changed -Action $action
# $job2 = Register-ObjectEvent -InputObject $watcher -EventName Created -Action $action
# $job3 = Register-ObjectEvent -InputObject $watcher -EventName Deleted -Action $action
# $job4 = Register-ObjectEvent -InputObject $watcher -EventName Renamed -Action $action
# $job5 = Register-ObjectEvent -InputObject $watcher -EventName Error -Action {
#     echo "Error!"
# }

$watcher.EnableRaisingEvents = $true

while ($true) {
    Receive-Job $job1
    sleep 1
}

# [Console]::ReadLine()
