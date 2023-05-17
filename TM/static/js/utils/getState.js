export const getState = async () => {
  let data = null;
  try {
    const res = await fetch("http://localhost:8000/api/state");
    data = await res.json();
  } catch (err) {
    console.error(err);
  }
  return data;
};
