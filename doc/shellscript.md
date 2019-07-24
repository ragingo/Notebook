<center><h1>shell script まとめ</h1></center>

Index
===
- [Index](#index)
- [パスワード生成](#%e3%83%91%e3%82%b9%e3%83%af%e3%83%bc%e3%83%89%e7%94%9f%e6%88%90)
- [ファイル一括処理](#%e3%83%95%e3%82%a1%e3%82%a4%e3%83%ab%e4%b8%80%e6%8b%ac%e5%87%a6%e7%90%86)
- [数列生成](#%e6%95%b0%e5%88%97%e7%94%9f%e6%88%90)
- [ネットワーク](#%e3%83%8d%e3%83%83%e3%83%88%e3%83%af%e3%83%bc%e3%82%af)
  - [WiFi](#wifi)

# パスワード生成

```sh
cat /dev/random | tr -dc "0-9A-Za-z#$_" | fold -w 32
```

# ファイル一括処理

```sh
out_dir=/mnt/e/temp
file_ext=mkv

for i in * ; do \
    out_name="${i%.*}" ;
    out_path="$out_dir/$out_name.$file_ext" ; \
    ffmpeg -i "$i" -map 0:0 -map 0:2 -c:v copy -c:a copy "$out_path" ; \
done
```

# 数列生成

```sh
seq value step end
```

=> value < end

```sh
seq 0 4 15
```

=> 0 4 8 12

# ネットワーク
## WiFi
### アクセスポイント一覧
```
nmcli dev wifi list
```

### アクセスポイント設定
```
sudo nmcli device wifi connect [ssid] password [password]
```

### cURL
#### ヒアドキュメント
```sh
curl -X POST https://aaaaa \
-d @- << EOF
{
    "a": {
        "b": "123"
    },
}
EOF
```