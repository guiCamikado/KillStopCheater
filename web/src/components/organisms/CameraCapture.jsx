import { useEffect, useRef, useState } from "react";
import { createWorker } from "tesseract.js";
import cv from "@techstark/opencv-js";
import { ocrSpace } from "ocr-space-api-wrapper";

const OCR_API_KEY = import.meta.env.OCR_API_KEY;


export default function CameraCapture() {
  const videoRef = useRef(null);
  const canvasRef = useRef(null);

  const [photo, setPhoto] = useState(null);
  const [delay, setDelay] = useState(2000);

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

        // captura da câmera
        ctx.drawImage(video, 0, 0, canvas.width, canvas.height);

        // OpenCV
        let src = cv.imread(canvas);
        let gray = new cv.Mat();
        let blur = new cv.Mat();
        let thresh = new cv.Mat();

        // converte para cinza
        cv.cvtColor(src, gray, cv.COLOR_RGBA2GRAY);
        // reduz ruído
        cv.GaussianBlur(gray, blur, new cv.Size(5, 5), 0);
        // preto e branco
        cv.threshold(blur, thresh, 0, 255, cv.THRESH_BINARY + cv.THRESH_OTSU);
        // inverter (muitos displays ficam melhores assim)
        cv.bitwise_not(thresh, thresh);
        // mostra resultado no mesmo canvas
        cv.imshow(canvas, thresh);

        const processedImage = canvas.toDataURL("image/jpeg");
        setPhoto(processedImage);

        src.delete();
        gray.delete();
        blur.delete();
        thresh.delete();

        resolve(processedImage);
      }, delay);
    });
  }

  async function readDisplay(image) {
    try {
      const result = await ocrSpace(
        image,
        {
          apiKey: OCR_API_KEY,
          language: "eng",
          OCREngine: "2",
          scale: true,
          isOverlayRequired: false,
        },
      );

      const texto = result?.ParsedResults?.[0]?.ParsedText || "";

      // deixa apenas números
      const numero = texto.replace(/[^0-9.-]/g, "").trim();

      console.log(numero);
      return numero;
    } catch (err) {
      console.log(err);
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
        className="col-span-1 h-40 rounded-xl bg-blue-600 hover:bg-blue-500 transition text-xl font-bold shadow-lg" // ✅ Fix 3
      >
        Tirar Foto
      </button>

      <div className="h-40 col-span-1 bg-zinc-800 rounded-xl border border-zinc-700 p-4 flex flex-col justify-center">
        <label className="text-sm text-zinc-400 mb-2">Picture Delay (ms)</label>
        <input
          type="number"
          value={delay}
          onChange={(e) => setDelay(Number(e.target.value))}
          className="bg-zinc-700 rounded-lg p-2 outline-none"
        />
      </div>

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
