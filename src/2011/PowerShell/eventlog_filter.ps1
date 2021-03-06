cls

$xpath = "*[System[EventID=4624 or EventID=4634]]"
Get-WinEvent -LogName Security -FilterXPath $xpath | %{
    $row = ([xml]$_.ToXml() | 
        select `
            @{Name="EventID";                   Expression={ $_.Event.System.EventID }},
            @{Name="TimeCreated";               Expression={ [DateTime]$_.Event.System.TimeCreated.SystemTime }},
            @{Name="LogonType";                 Expression={ $_.SelectSingleNode("//*[@Name='LogonType']")."#text" }},
            @{Name="SubjectUserName";           Expression={ $_.SelectSingleNode("//*[@Name='SubjectUserName']")."#text" }},
            @{Name="SubjectLogonId";            Expression={ $_.SelectSingleNode("//*[@Name='SubjectLogonId']")."#text" }},
            @{Name="TargetUserName";            Expression={ $_.SelectSingleNode("//*[@Name='TargetUserName']")."#text" }},
            @{Name="TargetLogonId";             Expression={ $_.SelectSingleNode("//*[@Name='TargetLogonId']")."#text" }},
            @{Name="LogonProcessName";          Expression={ $_.SelectSingleNode("//*[@Name='LogonProcessName']")."#text" }},
            @{Name="AuthenticationPackageName"; Expression={ $_.SelectSingleNode("//*[@Name='AuthenticationPackageName']")."#text" }},
            @{Name="LmPackageName";             Expression={ $_.SelectSingleNode("//*[@Name='LmPackageName']")."#text" }},
            @{Name="IpAddress";                 Expression={ $_.SelectSingleNode("//*[@Name='IpAddress']")."#text" }}
        )
}


