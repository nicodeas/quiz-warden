import { host } from "./config.js";

const loginUser = async () => {
  const username = document.getElementById("username");
  const password = document.getElementById("password");

  const credentials = {
    username: username.value,
    password: password.value,
  };
  // reset input values
  username.value = "";
  password.value = "";

  let res;
  try {
    res = await fetch(`http://${host}/api/login`, {
      headers: {
        Accept: "application/json",
        "Content-Type": "application/json",
      },
      method: "POST",
      body: JSON.stringify(credentials),
    });
  } catch (err) {
    console.log(err);
  }

  if (res.status === 200) {
    window.location.href = `http://${host}`;
  } else {
    const loginError = document.getElementById("login-error");
    loginError.innerText =
      "Failed to login: Invalid credentials";
  }
};

const logoutUser = async (e) => {
  try {
    await fetch(`http://${host}/api/logout`, {});
    window.location.href = `http://${host}/login`;
  } catch (err) {
    console.log(err);
  }
};

const submit = document.getElementById("submit")
if (submit) submit.addEventListener("click", loginUser);

const logout = document.getElementById("logout")
if (logout) logout.addEventListener("click", logoutUser);
