class LocalStorage {
  static setData(key, value) {
    localStorage.setItem(key, JSON.stringify(value));
  }

  static getData(key) {
    return JSON.parse(localStorage.getItem(key));
  }

  static handleEventInput(e, currentData, setValue) {
    const { name, value, type } = e.target;
    const parsed = type === "number" ? Number(value) : value;
    const newData = { ...currentData, [name]: parsed };
    setValue(newData);
    return newData;
  }

  static insertFakeValue(key, value, currentData, setValue) {
    setValue({ ...currentData, [key]: value });
  }
}

export default LocalStorage;