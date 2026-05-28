// Libs & React
import { useEffect, useState } from "react";
// Atoms
import ItemHolder from "../atoms/wrappers/ItemHolder";
import CentralTitle from "../atoms/titles/CentralTitle";
import DefaultInput from "../atoms/inputs/DefaultInput";
// Molecules
import CameraCapture from "../organisms/CameraCapture";
// JS Classes
import LocalStorage from "../../utils/LocalStorageSaver";
import HttpClient from "../../utils/httpClient";

export default function Homepage() {
  const [data, setData] = useState({});
  const [urlLink, setUrlLink] = useState("");

  const handleEventInput = (e) => {
    LocalStorage.handleEventInput(e, data, setData);
  };

  const sendActionTimeStamp = (key) => {
    let timeNow = performance.now() * 1000;
    LocalStorage.insertFakeValue(key, timeNow, data, setData);
  };

  const sendData = (urlLink, data) => {
    HttpClient.sendData(urlLink, data);
  };

  function handleSubmit(e) {
    e.preventDefault();
    console.log("Enviado");
  }

  return (
    <ItemHolder
      elements={
        <>
          <CentralTitle text={"Camera Timing Config"} />

          <form onSubmit={handleSubmit}>
            <div className="grid grid-cols-4 gap-4">
              {/* Hit Score */}
              <div className="flex items-center font-medium text-zinc-300">
                Tempo Alvo
              </div>
              <div className="col-span-3">
                <DefaultInput
                  type={"number"}
                  name={"hitscore"}
                  placeholder={"10000"}
                  onChange={handleEventInput}
                />
              </div>

              {/* Delay */}
              <CameraCapture
                insertFakeValue={sendActionTimeStamp}
                onPhotoTaken={() => sendActionTimeStamp("pictureTime")}
              />

              {/* Set Unity */}
              <div className="flex items-center font-medium text-zinc-300">
                Tempo na foto
              </div>

              <div className="col-span-3">
                <DefaultInput
                  type={"number"}
                  name={"pictureTimeFrame"}
                  placeholder={"10000"}
                  onChange={handleEventInput}
                />
              </div>

              {/* timeMissCorrection */}
              <div className="flex items-center font-medium text-zinc-300">
                Time Scored
              </div>

              <div className="col-span-3">
                <DefaultInput
                  type={"number"}
                  name={"timeScored"}
                  placeholder={"0"}
                  onChange={handleEventInput}
                />
              </div>

              {/* Send Address */}
              <div className="flex items-center font-medium text-zinc-300">
                Adress
              </div>

              <div className="col-span-3">
                <DefaultInput
                  type="text"
                  name="postAddress"
                  placeholder="192.168.0.1/source?"
                  onChange={(e) => {
                    handleEventInput(e);
                    setUrlLink("http://" + e.target.value);
                  }}
                />
              </div>

              <button
                onClick={() => {
                  const timeNow = performance.now() * 1000;

                  const finalData = {
                    ...data,
                    sendTime: timeNow,
                  };

                  LocalStorage.insertFakeValue(
                    "sendTime",
                    timeNow,
                    finalData,
                    setData,
                  );

                  sendData(urlLink, finalData);
                }}
                className="col-span-4 h-40 rounded-xl bg-blue-600 hover:bg-blue-500 transition text-xl font-bold shadow-lg"
              >
                Enviar
              </button>
            </div>
          </form>
        </>
      }
    />
  );
}
