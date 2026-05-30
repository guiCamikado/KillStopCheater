import { useEffect, useRef, useState } from "react";

const OCR_API_KEY = import.meta.env.VITE_OCR_API_KEY;

export default function CameraCapture({ insertFakeValue, onPhotoTaken }) {
  const videoRef = useRef(null);
  const canvasRef = useRef(null);

  const [photo, setPhoto] = useState(null);
  const [delay, setDelay] = useState(0);

  async function startCamera() {
    try {
      const stream = await navigator.mediaDevices.getUserMedia({
        video: { facingMode: "environment" },
      });
      videoRef.current.srcObject = stream;
    } catch (err) {
      console.error("Erro ao abrir câmera:", err);
    }
  }

  function stopCamera() {
    const stream = videoRef.current?.srcObject;
    if (!stream) return;
    stream.getTracks().forEach((track) => track.stop());
  }

  async function takePicture() {
    return new Promise((resolve) => {
      setTimeout(() => {
        const video = videoRef.current;
        const canvas = canvasRef.current;

        canvas.width = video.videoWidth;
        canvas.height = video.videoHeight;

        const ctx = canvas.getContext("2d");
        ctx.drawImage(video, 0, 0, canvas.width, canvas.height);

        onPhotoTaken?.(); // <-- timestamp capturado AQUI, junto com a foto

        const processedImage = canvas.toDataURL("image/jpeg");
        setPhoto(processedImage);

        resolve(processedImage);
      }, delay);
    });
  }

  async function readDisplay(imageDataURL) {
    // Converte dataURL para Blob (necessário para enviar como arquivo)
    const blob = await fetch(imageDataURL).then((res) => res.blob());

    const formData = new FormData();
    formData.append("file", blob, "image.jpg");
    formData.append("apikey", OCR_API_KEY);
    formData.append("language", "eng");
    formData.append("OCREngine", "2");
    formData.append("scale", "true");
    formData.append("isOverlayRequired", "false");

    try {
      const response = await fetch("https://api.ocr.space/parse/image", {
        method: "POST",
        body: formData,
      });

      const result = await response.json();
      const texto = result?.ParsedResults?.[0]?.ParsedText || "";
      const numero = texto.replace(/[^0-9.-]/g, "").trim();
      console.log(numero);
      return numero;
    } catch (err) {
      console.error("Erro na requisição OCR Space:", err);
    }
  }

  useEffect(() => {
    startCamera();
    return () => stopCamera();
  }, []);

  return (
    <div className="grid grid-cols-4 gap-4 col-span-4">
      <button
        onClick={async () => {
          const image = await takePicture();
          await readDisplay(image);
        }}
        className="col-span-2 h-40 rounded-xl bg-blue-600 hover:bg-blue-500 transition text-xl font-bold shadow-lg"
      >
        Tirar Foto
      </button>

      <video
        ref={videoRef}
        autoPlay
        playsInline
        className="col-span-1 rounded-xl border border-zinc-700"
      />

      <canvas ref={canvasRef} className="hidden" />

      {photo && (
        <img
          src={photo}
          alt="foto"
          className="col-span-1 rounded-xl border-2 border-red-500"
        />
      )}
    </div>
  );
}
