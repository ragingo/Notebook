cd C:\Users\Shouta\Desktop

$ipv4_header = @{
    version         = 0;
    header_length   = 0;
    tos             = 0;
    total_length    = 0;
    id              = 0;
    ctrl_flag       = 0;
    fragment_offset = 0;
    ttl             = 0;
    protocol        = 0;
    checksum        = 0;
    src_addr        = 0;
    dest_addr       = 0;
    option          = 0;
    data            = 0;
}

$protocols = @{}

$load_protocol_numbers = {
    cat ./protocol_numbers.txt | % {
        $cols = $_ -split "`t"
        $num  = [int]$cols[0]
        $name = $cols[1]
        $protocols.$num = $name
    }
}

&$load_protocol_numbers

$main = {

    $addr = [Net.IPAddress]::Parse("192.168.1.23")
    $sock = New-Object Net.Sockets.Socket("InterNetwork", "Raw", "IP")
    $sock.Bind((New-Object Net.IPEndPoint($addr, 0)))
    $sock.SetSocketOption("IP", "HeaderIncluded", $true)
    $sock.IOControl([Net.Sockets.IOControlCode]::ReceiveAll, @(1,0,0,0), @(0,0,0,0))

    $max = 60
    $curr = 0
    
    while ($curr -lt $max) {

        $buf = New-Object Byte[] 1024
        $len = $sock.Receive($buf, "None")
        
        $ipv4_header.version         = [Math]::Floor(($buf[0] / [Math]::Pow(2, 4)))
        $ipv4_header.header_length   = ($buf[0] -band 0x0f)
        $ipv4_header.tos             = $buf[1]
        $ipv4_header.total_length    = ($buf[2] * [Math]::Pow(2, 8)) -bor $buf[3]
        $ipv4_header.id              = ($buf[4] * [Math]::Pow(2, 8)) -bor $buf[5]
        $ipv4_header.ctrl_flag       = [Math]::Floor(($buf[6] / [Math]::Pow(2, 5)))
        $ipv4_header.fragment_offset = (($buf[6] -band 0x1f) * [Math]::Pow(2, 8)) -bor $buf[7]
        $ipv4_header.ttl             = $buf[8]
        $ipv4_header.protocol        = $protocols[[int]$buf[9]]
        $ipv4_header.checksum        = ($buf[10] * [Math]::Pow(2, 8)) -bor $buf[11]
        $ipv4_header.src_addr        = $buf[12..15] -join "."
        $ipv4_header.dest_addr       = $buf[16..19] -join "."

        echo "---------------------------------------------------------------------------"        
        echo ("{0} " -f (Get-Date))
        
        $ipv4_header
        
        sleep 1
        $curr++
    }
}

&$main