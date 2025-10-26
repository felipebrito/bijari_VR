# 📹 Vídeos Necessários para o Projeto

## ⚠️ ATENÇÃO: Arquivos de Vídeo Removidos

Os arquivos de vídeo foram removidos deste repositório para reduzir o tamanho. Você precisa adicionar seus próprios vídeos para o funcionamento do projeto.

## 📂 Localização dos Vídeos

### Para Quest 3 / Android
Coloque seus vídeos em uma das seguintes pastas no dispositivo:

1. **`/sdcard/Download/`** (recomendado)
2. **`/storage/emulated/0/Download/`**
3. **`/storage/self/primary/Download/`**
4. **`/sdcard/Movies/`**

### Para Unity Editor / Desktop
Coloque seus vídeos em:
```
socket-client/Assets/StreamingAssets/
```

## 🎬 Formato de Vídeo

- **Formato:** MP4
- **Codec:** H.264
- **Resolução:** 360° (preferencialmente 4K ou superior)
- **Nome sugestivo:** Nomes descritivos como `video_360_01.mp4`

## ✅ Vídeos Testados

- `Pierre_Final.mp4` - Vídeo 360° de teste (removido para redução de tamanho)

## 📝 Configuração no Unity

No `VRManager.cs`, configure os arquivos de vídeo no array:

```csharp
public string[] videoFiles = { "seu_video.mp4", "outro_video.mp4" };
```

## 🔗 Adicionar ao Projeto

1. Copie seus vídeos para a pasta apropriada
2. Certifique-se de que o `VIDEO_PLACEHOLDER.md` está presente
3. Os vídeos devem ter permissão de leitura no Android

## ⚠️ Importante

- Não adicione vídeos pesados ao repositório GitHub
- Use um serviço de compartilhamento separado para vídeos de produção
- Mantenha vídeos de teste pequenos (máximo 50MB)

