cls

ls -r 'F:\' |
    ? { $_.FullName -match "^.+\.cs$" } |
    ? {
        $bytes = cat $_.FullName -total 3 -enc Byte
        if ($bytes -eq $null -or $bytes.Length -lt 3) {
            return $null
        }
        if (-not ($bytes[0] -eq 0xef -and
                  $bytes[1] -eq 0xbb -and
                  $bytes[2] -eq 0xbf)) {
            $_
        }
    } |
    select FullName