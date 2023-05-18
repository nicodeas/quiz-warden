export const renderAnswer = (data) => {
  if (data.type === "image") {

    const imageOutputElement = document.getElementById("output-image");
    imageOutputElement.src = `data:image/png;base64, ${data.answer}`;
  }
};
