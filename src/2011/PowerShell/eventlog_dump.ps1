cls

function Get-PropNames($obj) {
    return ($obj | Get-Member -Type Property | select Name | %{ $_.Name })
}

function Get-EventLogObject([xml]$doc) {
    $obj = New-Object PSObject
    
    # /Event/System
    (Get-PropNames $doc.Event.System) | %{
        $name = $_
        $prop = $doc.Event.System."$name"
        
        if($prop.GetType() -eq [Xml.XmlElement]) {
            (Get-PropNames $prop) | %{ 
                $obj | Add-Member NoteProperty "$name#$_" $prop."$_"
            }
        }
        else {
            $obj | Add-Member NoteProperty $_ $prop
        }
    }
    # /Event/EventData
    if($doc.Event.EventData -ne $null) {
        ($doc.Event.EventData.Data | select Name,"#text") | %{
            $obj | Add-Member NoteProperty $_.Name $_."#text"
        }
    }
    return $obj
}

function Show-EventLog([string]$xPath) {
    Get-WinEvent -LogName Security -FilterXPath $xPath | %{
        Get-EventLogObject ([xml]$_.ToXml())
    }
}

Show-EventLog "*"