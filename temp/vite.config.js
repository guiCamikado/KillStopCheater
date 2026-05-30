import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";
import tailwindcss from "@tailwindcss/vite";
import { VitePWA } from "vite-plugin-pwa";

export default defineConfig({
  plugins: [
    react(),
    tailwindcss(),
    VitePWA({
      workbox: {
        maximumFileSizeToCacheInBytes: 12 * 1024 * 1024,
      },
      registerType: "autoUpdate",
      manifest: {
        name: "Meu App",
        short_name: "MeuApp",
        start_url: "/",
        display: "standalone",
        background_color: "#ffffff",
        theme_color: "#ffffff",
        icons: [
          {
            src: "icon.png",
            sizes: "192x192",
            type: "image/png",
          },
        ],
      },
    }),
  ],

  server: {
    watch: {
      usePolling: true,
      interval: 100,
    },
    hmr: {
      overlay: true,
    },
  },
});
