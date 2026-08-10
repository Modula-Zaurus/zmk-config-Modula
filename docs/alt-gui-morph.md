# Alt/GUI key morph behavior

`&agm` は 2 個のエンコード済みキーコードを引数に取り、Alt または GUI が明示的に押されている場合だけ第 2 引数を送る behavior です。レイヤーは変更しません。

```dts
&agm C_MUTE LC(TAB)
```

この例では、通常時は Mute、Alt または GUI と同時なら Ctrl+Tab を送ります。通常のキーコード、Consumer キーコード、`LC(TAB)` のような修飾付きキーコードを指定できます。

現在は `config/Modula.keymap` の左下から 2 個目のキーをこの behavior の動作確認用に置き換えています。
