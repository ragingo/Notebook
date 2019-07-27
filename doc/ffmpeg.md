<center><h1>ffmpeg まとめ</h1></center>

Index
===
- [Index](#index)
- [ファイル操作](#%e3%83%95%e3%82%a1%e3%82%a4%e3%83%ab%e6%93%8d%e4%bd%9c)
  - [連結](#%e9%80%a3%e7%b5%90)
  - [カット](#%e3%82%ab%e3%83%83%e3%83%88)
  - [メタデータ](#%e3%83%a1%e3%82%bf%e3%83%87%e3%83%bc%e3%82%bf)
- [トランスコード](#%e3%83%88%e3%83%a9%e3%83%b3%e3%82%b9%e3%82%b3%e3%83%bc%e3%83%89)
  - [ストリーム選択](#%e3%82%b9%e3%83%88%e3%83%aa%e3%83%bc%e3%83%a0%e9%81%b8%e6%8a%9e)
  - [可変ビットレート](#%e5%8f%af%e5%a4%89%e3%83%93%e3%83%83%e3%83%88%e3%83%ac%e3%83%bc%e3%83%88)
- [ダウンロード](#%e3%83%80%e3%82%a6%e3%83%b3%e3%83%ad%e3%83%bc%e3%83%89)
  - [HLS](#hls)

# ファイル操作

## 連結

https://trac.ffmpeg.org/wiki/Concatenate

```
ffmpeg -f concat -safe 0 -i list.txt -c copy dst.mp4
```

list.txt
```
file /home/user1/absolute.mp4
file relative.mp4
```

## カット

https://trac.ffmpeg.org/wiki/Seeking

```
ffmpeg -i src.mp4 -ss 00:00:00 -t 00:00:01 dst.mp4
```

## メタデータ

### ID3 バージョン変更

v2.x (vX) -> 2.2 (v2)

```
ffmpeg -i src.mp3 -id3v2_version 2 dst.mp3 -y
```

### 書き換え

```
ffmpeg -i src.mp3 -c:a copy -metadata TALB=AbcAlbum dst.mp3
ffmpeg -i src.mp3 -c:a copy -metadata album=AbcAlbum dst.mp3
```

### ダンプ出力

```
ffmpeg -i src.mp3 -f ffmetadata
```


# トランスコード

## ストリーム選択

(例) -map 使用例
- Stream 0:0 Video (残す)
- Stream 0:1 Audio(en) (消す)
- Stream 0:2 Audio(jp) (残す)

```
ffmpeg -i src.mp4 -map 0:0 -map 0:2 -c:v copy -c:a copy dst.mp4
```

## 可変ビットレート

```
ffmpeg -i src.mp4 -c:v nvenc_hevc -rc vbr_2pass -rc-lookahead 20 -gpu any -b:v  -c:a copy dst.mp4
```

```
ffmpeg -i src.mp4 -c:v nvenc_h265 -c:a copy -b:v 1M -minrate 750K -maxrate 1.5M -threads 8 dst.mp4
```

# ダウンロード

## HLS

```
ffmpeg -protocol_whitelist file,http,https,tcp,tls,crypto -i master.m3u8 -c copy -map p:3 dst.mp4
```