
# Social Network

This project is a Qt-based social media application that allows users to interact, follow each other, post, serch, explorer, exchange messages, and much more.

## Images of Social Network

![Login Page](/images/login_page.png)

![Get Informaiton](/images/get_information.png)

![My Page](/images/my_page.png)

![Setting](/images/setting.png)

![Followings](/images/followings.png)

![Followings Posts](/images/followings_posts.png)

![Search](/images/search.png)

![Messages](/images/messages.png)


## Features

- **User Authentication**: Manage user login sessions, authentication, and registration.
- **Profile Management**: Enable users to create, view, and edit their profiles, including personal information and privacy settings.
- **Social Interactions**: Users can search for others, follow/unfollow, like/unlike posts, and view followers and followings.
- **Messaging**: Feature to view and manage received messages, with options to accept or reject message requests.
- **Content Management**: Users can add, edit, and delete their posts, and view posts from their network.
- **Recommendations**: Suggests users to follow based on specific criteria.
- **Settings and Admin Control**: Provides interfaces for account settings, password changes, and administrative controls.
- **Explorer**: Allows users to explore posts from the people they follow.


## Prerequisites

Before you begin, ensure you have the following requirements installed:
- Qt 5.x or later
- C++ compiler compatible with C++11 or later
- Qt Creator (optional for building and running the project)

## Getting Started

To get started with the Social Network App, clone this repository to your local machine and open the project in Qt Creator or build it using `qmake` and `make`.

### Cloning the Repository

```bash
git clone https://github.com/MohammadMazrouei/Social-Network.git
cd Social-Network
```

### Building the Project

#### Using Qt Creator
1. Open `socialmedia.pro` with Qt Creator.
2. Configure the project with your preferred kit.
3. Build and run the project by clicking the Run button.

#### Using Command Line
In the project directory, run:

```bash
qmake socialmedia.pro
make
```

This will compile the project. You can then run the Social Network executable generated in the build directory.


## Additional Notes

- Adjust database paths and setup instructions based on your environment.
- Further administrative features may require additional development based on the `admin_control.cpp` file.


## Contributing

Contributions to the Social Network project are welcome. Please feel free to fork the repository, make your changes, and submit a pull request.

## License

This project is open source and available under the [MIT License](LICENSE).

## Acknowledgments

- Thanks to the Qt framework for providing the tools to build the GUI.
- Thanks to all contributors.
