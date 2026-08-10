# Modula のモジュール snippets

現段階では右半分を central、左半分を peripheral とする既存の役割分担を維持します。モジュールは snippet を組み合わせて選択します。

- トラックボール: `trackball_pat9125`、`trackball_paw3222`、`pmw3610_alt`
- EC11 スロット 1: `encoder_ec11_left`（P1.13 / P1.14）
- EC11 スロット 2: `encoder_ec11_right`（P1.12 / P1.15）

PAT9125 と EC11 は別の GPIO/I2C 資源を使用するため、たとえば左 peripheral では次のように同時指定できます。

```yaml
snippet: trackball_pat9125 encoder_ec11_left
```

EC11 snippets は 2 つとも指定できます。ただし `encoder_ec11_right` は P1.12 を使うため、同じピンを chip-select に使う SPI トラックボールとは併用しません。
