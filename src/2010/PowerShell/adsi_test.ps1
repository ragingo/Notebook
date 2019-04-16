
$users =[ADSI]"WinNT://HomePC"
[void]$users.Children.SchemaFilter.Add("user")

$users.Children |
  select `
    Name,
    @{ 
      Name = "LastPasswordChange";
      Expression={ (Get-Date).AddSeconds(-1 * $_.PasswordAge).ToString("yyyy/MM/dd HH:mm:ss") }
    },
    @{ 
      Name = "LastPasswordChange2";
      Expression={ (Get-Date).AddSeconds($_.PasswordAge) }
    },
    PasswordAge |
      ft -auto






function Get-Inventory {
	process {
		$os = Get-WmiObject Win32_OperatingSystem -computer $_
		$bios = Get-WmiObject Win32_BIOS -computer $_
		$output = New-Object PSObject
		$output | Add-Member NoteProperty ComputerName $_
		$output | Add-Member NoteProperty SPVer ($os.ServicePackMajorVersion)
		$output | Add-Member NoteProperty BuildNo ($os.BuildNumber)
		$output | Add-Member NoteProperty BIOSSerial ($bios.SerialNumber)
		return $output
	}
}
@(hostname) | Get-Inventory | ft -a

