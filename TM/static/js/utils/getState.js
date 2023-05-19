import { host } from "../config.js";

export const getState = async () => {
  let data = null;
  try {
    const res = await fetch(`http://${host}/api/state`);
    data = await res.json();
  } catch (err) {
    console.error(err);
  }
  return data;
};
