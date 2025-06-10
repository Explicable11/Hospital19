import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.*;
import java.sql.*;
import java.util.List;
import java.util.ArrayList;

public class HospitalManagementSystem extends JFrame {
    private static final String url = "ur url";
    private static final String dbUsername = "root";
    private static final String dbPassword = "";
    private Connection connection;

    private float headingOpacity = 0.0f;  // initial opacity for fade-in effect using this lines
    private Timer fadeInTimer;

    private int backgroundOffsetX = 0;
    private int backgroundOffsetY = 0;

    public HospitalManagementSystem() {
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            connection = DriverManager.getConnection(url, dbUsername, dbPassword);
        } catch (Exception e) {
            e.printStackTrace();
        }

        setTitle("Hospital Management System");
        setSize(500, 600); // Adjust the size as needed
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        // create a backgroundPanel with the image path
        BackgroundPanel backgroundPanel = new BackgroundPanel("image path as png");
        backgroundPanel.setLayout(new BorderLayout());

        // header Panel
        JPanel headerPanel = new JPanel();
        headerPanel.setOpaque(false); // Make header transparent so background shows
        JLabel headerLabel = new JLabel("Hospital Management System");
        headerLabel.setFont(new Font("Arial", Font.BOLD, 48));
        headerLabel.setForeground(Color.black); // White color for contrast
        headerLabel.setOpaque(false);  // Set label to be transparent initially
        headerLabel.setForeground(new Color(0, 0, 0, (int)(headingOpacity * 255)));  // Set initial transparency

        headerPanel.add(headerLabel);

        // button panel
        JPanel buttonPanel = new JPanel(new GridBagLayout());
        buttonPanel.setOpaque(false); // Make button panel transparent
        buttonPanel.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20));

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(10, 10, 10, 10);
        gbc.fill = GridBagConstraints.HORIZONTAL;

        // buttons
        JButton addPatientButton = createStyledButton("Add Patient");
        JButton viewPatientsButton = createStyledButton("View Patients");
        JButton viewDoctorsButton = createStyledButton("View Doctors");
        JButton viewScheduleButton = createStyledButton("View Doctor Schedule");
        JButton bookAppointmentButton = createStyledButton("Book Appointment");
        JButton viewAppointmentsButton = createStyledButton("View Appointments");
        JButton viewAppointmentHistoryButton = createStyledButton("View Appointment History");
        JButton deletePatientButton = createStyledButton("Delete Patient");
        JButton exitButton = createStyledButton("Exit");

        // add actionListeners to each button (existing code remains unchanged)
        addPatientButton.addActionListener(e -> addPatient());
        viewPatientsButton.addActionListener(e -> viewPatients());
        viewDoctorsButton.addActionListener(e -> viewDoctors());
        viewScheduleButton.addActionListener(e -> viewSchedule());
        bookAppointmentButton.addActionListener(e -> bookAppointment());
        viewAppointmentsButton.addActionListener(e -> viewAppointments());
        viewAppointmentHistoryButton.addActionListener(e -> {
            String patientIdInput = JOptionPane.showInputDialog("Enter Patient ID:");
            try {
                int patientId = Integer.parseInt(patientIdInput);
                viewAppointmentHistory(patientId);
            } catch (NumberFormatException ex) {
                JOptionPane.showMessageDialog(null, "Invalid Patient ID. Please enter a valid number.", "Error", JOptionPane.ERROR_MESSAGE);
            }
        });
        deletePatientButton.addActionListener(e -> deletePatient());
        exitButton.addActionListener(e -> System.exit(0));

        // add buttons to panel
        gbc.gridx = 0;
        gbc.gridy = 0;
        buttonPanel.add(addPatientButton, gbc);

        gbc.gridy++;
        buttonPanel.add(viewPatientsButton, gbc);

        gbc.gridy++;
        buttonPanel.add(viewDoctorsButton, gbc);

        gbc.gridy++;
        buttonPanel.add(viewScheduleButton, gbc);

        gbc.gridy++;
        buttonPanel.add(bookAppointmentButton, gbc);

        gbc.gridy++;
        buttonPanel.add(viewAppointmentsButton, gbc);

        gbc.gridy++;
        buttonPanel.add(viewAppointmentHistoryButton, gbc);

        gbc.gridy++;
        buttonPanel.add(deletePatientButton, gbc);

        gbc.gridy++;
        buttonPanel.add(exitButton, gbc);

        // Add the button panel to the background panel
        backgroundPanel.add(buttonPanel, BorderLayout.CENTER);

        // Add panels to the BackgroundPanel
        backgroundPanel.add(headerPanel, BorderLayout.NORTH);
        backgroundPanel.add(buttonPanel, BorderLayout.CENTER);

        // Set BackgroundPanel as content pane
        setContentPane(backgroundPanel);

        // Start the fade-in animation for the heading
        startFadeInAnimation(headerLabel);

        // Add MouseMotionListener for parallax effect
        backgroundPanel.addMouseMotionListener(new MouseMotionAdapter() {
            @Override
            public void mouseMoved(MouseEvent e) {
                // Parallax effect: Move the background in relation to the mouse position
                backgroundOffsetX = (e.getX() / 30);  // Adjust 30 for the effect speed
                backgroundOffsetY = (e.getY() / 30);  // Adjust 30 for the effect speed
                repaint();
            }
        });

        // Add WindowListener for resizing effect
        addComponentListener(new ComponentAdapter() {
            public void componentResized(ComponentEvent componentEvent) {
                // Adjust background offset on resize for consistent parallax effect
                backgroundOffsetX = getWidth() / 30;
                backgroundOffsetY = getHeight() / 30;
                repaint();
            }
        });
    }

    private void startFadeInAnimation(JLabel headerLabel) {
        fadeInTimer = new Timer(30, e -> {
            // Increase the opacity of the header text
            headingOpacity += 0.05f;  // Increase the opacity by 5% per tick
            if (headingOpacity >= 1.0f) {
                headingOpacity = 1.0f; // Cap it at fully opaque
                fadeInTimer.stop(); // Stop the timer once the opacity is 100%
            }

            // Update the alpha of the heading label
            headerLabel.setForeground(new Color(0, 0, 0, (int)(headingOpacity * 255)));  // Update opacity
            repaint(); // Trigger a repaint to apply the updated opacity
        });
        fadeInTimer.start();  // Start the fade-in effect
    }

    private JButton createStyledButton(String text) {
        JButton button = new JButton(text);
        button.setFont(new Font("Arial", Font.BOLD, 18));
        button.setForeground(Color.WHITE);
        button.setBackground(new Color(0, 123, 255));
        button.setFocusPainted(false);
        button.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(new Color(0, 123, 255), 8),
                BorderFactory.createEmptyBorder(5, 15, 5, 15)
        ));
        return button;
    }

    // Custom JPanel Class for full-size background image
    class BackgroundPanel extends JPanel {
        private Image backgroundImage;

        public BackgroundPanel(String filePath) {
            backgroundImage = new ImageIcon(filePath).getImage();
        }

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            // Apply the parallax effect by shifting the background based on the offsets
            g.drawImage(backgroundImage, backgroundOffsetX, backgroundOffsetY, getWidth(), getHeight(), this);
        }
    }


    public void addPatient() {
        JTextField nameField = new JTextField(10);
        JTextField ageField = new JTextField(10);
        JTextField genderField = new JTextField(10);
        JPanel panel = new JPanel();
        panel.add(new JLabel("Patient Name:"));
        panel.add(nameField);
        panel.add(new JLabel("Patient Age:"));
        panel.add(ageField);
        panel.add(new JLabel("Patient Gender:"));
        panel.add(genderField);

        int option = JOptionPane.showConfirmDialog(this, panel, "Add Patient", JOptionPane.OK_CANCEL_OPTION);
        if (option == JOptionPane.OK_OPTION) {
            String name = nameField.getText();
            int age;
            String gender = genderField.getText();

            if (name.isEmpty() || gender.isEmpty() || !isValidGender(gender)) {
                JOptionPane.showMessageDialog(this, "Please enter valid patient details.", "Input Error", JOptionPane.ERROR_MESSAGE);
                return;
            }

            try {
                age = Integer.parseInt(ageField.getText());
            } catch (NumberFormatException e) {
                JOptionPane.showMessageDialog(this, "Please enter a valid age.", "Input Error", JOptionPane.ERROR_MESSAGE);
                return;
            }

            try {
                String query = "INSERT INTO patients(Name, Age, Gender) VALUES(?, ?, ?)";
                try (PreparedStatement preparedStatement = connection.prepareStatement(query)) {
                    preparedStatement.setString(1, name);
                    preparedStatement.setInt(2, age);
                    preparedStatement.setString(3, gender);
                    int affectedRows = preparedStatement.executeUpdate();
                    if (affectedRows > 0) {
                        JOptionPane.showMessageDialog(this, "Patient added successfully!");
                    } else {
                        JOptionPane.showMessageDialog(this, "Failed to add patient!");
                    }
                }
            } catch (SQLException e) {
                JOptionPane.showMessageDialog(this, "Error adding patient: " + e.getMessage(), "Database Error", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    public void viewPatients() {
        String query = "SELECT * FROM patients";
        StringBuilder patientList = new StringBuilder();
        patientList.append("+------------+--------------------+----------+------------+\n");
        patientList.append("| Patient Id | Name               | Age      | Gender     |\n");
        patientList.append("+------------+--------------------+----------+------------+\n");

        try (PreparedStatement preparedStatement = connection.prepareStatement(query);
             ResultSet resultSet = preparedStatement.executeQuery()) {
            while (resultSet.next()) {
                int id = resultSet.getInt("id");
                String name = resultSet.getString("Name");
                int age = resultSet.getInt("age");
                String gender = resultSet.getString("Gender");
                patientList.append(String.format("| %-10s | %-18s | %-8s | %-10s |\n", id, name, age, gender));
                patientList.append("+------------+--------------------+----------+------------+\n");
            }
            JOptionPane.showMessageDialog(this, patientList.toString(), "Patient List", JOptionPane.INFORMATION_MESSAGE);
        } catch (SQLException e) {
            JOptionPane.showMessageDialog(this, "Error retrieving patients: " + e.getMessage(), "Database Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    public void viewDoctors() {
        String query = "SELECT * FROM doctors";
        StringBuilder doctorList = new StringBuilder();
        doctorList.append("+------------+--------------------+----------+------------+\n");
        doctorList.append("| Doctor Id  | Name               |  specialization|\n");
        doctorList.append("+------------+--------------------+----------+------------+\n");

        try (PreparedStatement preparedStatement = connection.prepareStatement(query);
             ResultSet resultSet = preparedStatement.executeQuery()) {
            while (resultSet.next()) {
                int id = resultSet.getInt("id");
                String name = resultSet.getString("Name");
                String  specialization = resultSet.getString("Specialization");
                doctorList.append(String.format("| %-10s | %-18s | %-8s |\n", id, name,specialization));
                doctorList.append("+------------+--------------------+----------+------------+\n");
            }
            JOptionPane.showMessageDialog(this, doctorList.toString(), "Doctor List", JOptionPane.INFORMATION_MESSAGE);
        } catch (SQLException e) {
            JOptionPane.showMessageDialog(this, "Error retrieving doctors: " + e.getMessage(), "Database Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    public void viewSchedule() {
        // Define column names for the table
        String[] columns = {"Doctor ID", "Schedule", "Time Slot"};

        // Query to get the doctor schedule details
        String query = "SELECT doctor_id, Schedule, Time_slot FROM doctor_schedule";

        try (Statement statement = connection.createStatement();
             ResultSet resultSet = statement.executeQuery(query)) {

            // Get the data from the database and store it in a list of rows
            List<Object[]> data = new ArrayList<>();
            while (resultSet.next()) {
                int doctorId = resultSet.getInt("Doctor_id");
                String schedule = resultSet.getString("Schedule");
                String timeSlot = resultSet.getString("Time_slot");
                data.add(new Object[]{doctorId, schedule, timeSlot});
            }

            // If no data found, inform the user
            if (data.isEmpty()) {
                JOptionPane.showMessageDialog(this, "No doctor schedules found.", "Information", JOptionPane.INFORMATION_MESSAGE);
                return;
            }

            // Create a table model with the retrieved data
            DefaultTableModel tableModel = new DefaultTableModel(data.toArray(new Object[0][]), columns);

            // Create a JTable with the model
            JTable table = new JTable(tableModel);
            table.setFillsViewportHeight(true);  // Make table fill the panel

            // Add the table to a JScrollPane to make it scrollable
            JScrollPane scrollPane = new JScrollPane(table);

            // Create a dialog to display the table
            JOptionPane.showMessageDialog(this, scrollPane, "Doctor Schedule View", JOptionPane.PLAIN_MESSAGE);

        } catch (SQLException e) {
            JOptionPane.showMessageDialog(this, "Error loading doctor schedule: " + e.getMessage(), "Database Error", JOptionPane.ERROR_MESSAGE);
        }
    }


    public void bookAppointment() {
        JTextField patientIdField = new JTextField(10);
        JTextField doctorIdField = new JTextField(10);
        JTextField dateField = new JTextField(10);
        JPanel panel = new JPanel();
        panel.add(new JLabel("Patient ID:"));
        panel.add(patientIdField);
        panel.add(new JLabel("Doctor ID:"));
        panel.add(doctorIdField);
        panel.add(new JLabel("Date (YYYY-MM-DD):"));
        panel.add(dateField);

        int option = JOptionPane.showConfirmDialog(this, panel, "Book Appointment", JOptionPane.OK_CANCEL_OPTION);
        if (option == JOptionPane.OK_OPTION) {
            try {
                int patientId = Integer.parseInt(patientIdField.getText());
                int doctorId = Integer.parseInt(doctorIdField.getText());
                String date = dateField.getText();

                String query = "INSERT INTO appointments(patient_id, doctor_id, appointment_date) VALUES(?, ?, ?)";
                try (PreparedStatement preparedStatement = connection.prepareStatement(query)) {
                    preparedStatement.setInt(1, patientId);
                    preparedStatement.setInt(2, doctorId);
                    preparedStatement.setString(3, date);
                    preparedStatement.executeUpdate();
                    JOptionPane.showMessageDialog(this, "Appointment booked successfully!");
                }
            } catch (Exception e) {
                JOptionPane.showMessageDialog(this, "Error booking appointment: " + e.getMessage(), "Database Error", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private boolean isValidGender(String gender) {
        return gender.equalsIgnoreCase("Male") || gender.equalsIgnoreCase("Female") || gender.equalsIgnoreCase("Other");
    }
    // Method to view appointments
    public void viewAppointments() {
        String query = "SELECT a.id AS appointment_id, p.name AS patient_name, d.name AS doctor_name, a.appointment_date " +
                "FROM appointments a " +
                "JOIN patients p ON a.patient_id = p.id " +
                "JOIN doctors d ON a.doctor_id = d.id";
        StringBuilder appointmentList = new StringBuilder();
        appointmentList.append("+------------------+--------------------+-------------------+-----------------+\n");
        appointmentList.append("| Appointment Id  | Patient Name       | Doctor Name       | Appointment Date |\n");
        appointmentList.append("+------------------+--------------------+-------------------+-----------------+\n");

        try (PreparedStatement preparedStatement = connection.prepareStatement(query);
             ResultSet resultSet = preparedStatement.executeQuery()) {
            while (resultSet.next()) {
                int appointmentId = resultSet.getInt("appointment_id");
                String patientName = resultSet.getString("patient_name");
                String doctorName = resultSet.getString("doctor_name");
                String appointmentDate = resultSet.getString("appointment_date");
                appointmentList.append(String.format("| %-16s | %-18s | %-18s | %-15s |\n", appointmentId, patientName, doctorName, appointmentDate));
                appointmentList.append("+------------------+--------------------+-------------------+-----------------+\n");
            }
            JOptionPane.showMessageDialog(this, appointmentList.toString(), "Appointment List", JOptionPane.INFORMATION_MESSAGE);
        } catch (SQLException e) {
            JOptionPane.showMessageDialog(this, "Error retrieving appointments: " + e.getMessage(), "Database Error", JOptionPane.ERROR_MESSAGE);
        }
    }
    public void viewAppointmentHistory(int patientId) {
        // SQL query to fetch appointments along with the doctor's name and specialization
        String query = "SELECT a.id AS appointment_id, d.name AS doctor_name, d.specialization AS doctor_specialization, a.appointment_date " +
                "FROM appointments a " +
                "JOIN doctors d ON a.doctor_id = d.id " +
                "WHERE a.patient_id = ?";

        StringBuilder appointmentHistory = new StringBuilder();
        appointmentHistory.append("+------------------+-------------------+-------------------+-----------------+\n");
        appointmentHistory.append("| Appointment Id  | Doctor Name       | Specialization    | Appointment Date |\n");
        appointmentHistory.append("+------------------+-------------------+-------------------+-----------------+\n");

        try (PreparedStatement preparedStatement = connection.prepareStatement(query)) {
            preparedStatement.setInt(1, patientId);  // Set the patientId as parameter for the query
            ResultSet resultSet = preparedStatement.executeQuery();

            // Process the result set and format the data into the table structure
            while (resultSet.next()) {
                int appointmentId = resultSet.getInt("appointment_id");
                String doctorName = resultSet.getString("doctor_name");
                String doctorSpecialization = resultSet.getString("doctor_specialization");
                String appointmentDate = resultSet.getString("appointment_date");

                // Append each appointment record in a formatted way
                appointmentHistory.append(String.format("| %-16d | %-18s | %-18s | %-15s |\n",
                        appointmentId, doctorName, doctorSpecialization, appointmentDate));
            }
            appointmentHistory.append("+------------------+-------------------+-------------------+-----------------+\n");

            // Display the appointment history in a dialog box
            JOptionPane.showMessageDialog(this, appointmentHistory.toString(), "Appointment History", JOptionPane.INFORMATION_MESSAGE);
        } catch (SQLException e) {
            // Display an error message in case of SQL issues
            JOptionPane.showMessageDialog(this, "Error retrieving appointment history: " + e.getMessage(), "Database Error", JOptionPane.ERROR_MESSAGE);
        }
    }


    public void deletePatient() {
        JTextField patientIdField = new JTextField(10);
        JPanel panel = new JPanel();
        panel.add(new JLabel("Enter Patient ID to delete:"));
        panel.add(patientIdField);

        int option = JOptionPane.showConfirmDialog(this, panel, "Delete Patient", JOptionPane.OK_CANCEL_OPTION);
        if (option == JOptionPane.OK_OPTION) {
            try {
                int patientId = Integer.parseInt(patientIdField.getText());

                // Check for dependent records in other tables (example: appointments)
                String checkQuery = "SELECT COUNT(*) FROM appointments WHERE patient_id = ?";
                try (PreparedStatement checkStatement = connection.prepareStatement(checkQuery)) {
                    checkStatement.setInt(1, patientId);
                    ResultSet rs = checkStatement.executeQuery();
                    rs.next();
                    int count = rs.getInt(1);

                    if (count > 0) {
                        JOptionPane.showMessageDialog(this, "This patient has dependent records (appointments). Cannot delete.", "Error", JOptionPane.ERROR_MESSAGE);
                    } else {
                        // Proceed with deletion if no dependencies
                        String deleteQuery = "DELETE FROM patients WHERE id = ?";
                        try (PreparedStatement deleteStatement = connection.prepareStatement(deleteQuery)) {
                            deleteStatement.setInt(1, patientId);
                            int affectedRows = deleteStatement.executeUpdate();

                            if (affectedRows > 0) {
                                JOptionPane.showMessageDialog(this, "Patient deleted successfully!");
                            } else {
                                JOptionPane.showMessageDialog(this, "Patient ID not found.", "Error", JOptionPane.ERROR_MESSAGE);
                            }
                        }
                    }
                }
            } catch (NumberFormatException e) {
                JOptionPane.showMessageDialog(this, "Please enter a valid Patient ID.", "Input Error", JOptionPane.ERROR_MESSAGE);
            } catch (SQLException e) {
                JOptionPane.showMessageDialog(this, "Error deleting patient: " + e.getMessage(), "Database Error", JOptionPane.ERROR_MESSAGE);
            }


        }
    }


    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new LoginPage().setVisible(true));
    }
}


class LoginPage extends JFrame {
    private static final String url = "ur url";
    private static final String dbUsername = "root";
    private static final String dbPassword = "password";
    private Connection connection;

    public LoginPage() {
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            connection = DriverManager.getConnection(url, dbUsername, dbPassword);
        } catch (Exception e) {
            e.printStackTrace();
        }

        // Set up frame
        setTitle("Hospital Management System - Login");
        setSize(400, 400);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        // Background panel with image
        BackgroundPanel backgroundPanel = new BackgroundPanel("image path");
        backgroundPanel.setLayout(new BorderLayout());

        // Transparent panel for login form components
        JPanel formPanel = new JPanel(new GridBagLayout());
        formPanel.setOpaque(false); // Make the panel transparent
        formPanel.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20));

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(10, 10, 10, 10);
        gbc.fill = GridBagConstraints.HORIZONTAL;

        // Heading label
        JLabel headingLabel = new JLabel("Welcome to our Hospital");
        headingLabel.setFont(new Font("Arial", Font.BOLD, 48));
        headingLabel.setForeground(Color.black);  // Set text color to white

        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.gridwidth = 2;
        gbc.anchor = GridBagConstraints.CENTER;
        formPanel.add(headingLabel, gbc);

        // Username and Password labels and fields
        JLabel userLabel = new JLabel("Username:");
        userLabel.setFont(new Font("Arial", Font.BOLD, 16));
        userLabel.setForeground(Color.BLACK);  // Set text color to white

        JTextField userField = new JTextField(15);
        userField.setForeground(Color.BLACK);  // Set text color to white
        userField.setBackground(new Color(255, 255, 255)); // Set a dark background

        JLabel passLabel = new JLabel("Password:");
        passLabel.setFont(new Font("Arial", Font.BOLD, 16));
        passLabel.setForeground(Color.BLACK);  // Set text color to white

        JPasswordField passField = new JPasswordField(15);
        passField.setForeground(Color.BLACK);  // Set text color to white
        passField.setBackground(new Color(255, 255, 255)); // Set a dark background

        JButton loginButton = createStyledButton("Login");

        // Forgot Password button
        JButton forgotPasswordButton = new JButton("Forgot Password?");
        forgotPasswordButton.setFont(new Font("Arial", Font.BOLD, 16));
        forgotPasswordButton.setForeground(Color.BLACK);  // Set text color to black
        forgotPasswordButton.setBackground(Color.WHITE);  // Set background color to white
        forgotPasswordButton.setFocusPainted(false);
        forgotPasswordButton.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(Color.BLACK, 3),  // Black border
                BorderFactory.createEmptyBorder(5, 15, 5, 15)));

                // Add components to the form panel
        gbc.gridwidth = 1;
        gbc.anchor = GridBagConstraints.WEST;
        gbc.gridx = 0;
        gbc.gridy = 1;
        formPanel.add(userLabel, gbc);
        gbc.gridx = 1;
        formPanel.add(userField, gbc);
        gbc.gridx = 0;
        gbc.gridy = 2;
        formPanel.add(passLabel, gbc);
        gbc.gridx = 1;
        formPanel.add(passField, gbc);
        gbc.gridx = 0;
        gbc.gridy = 3;
        gbc.gridwidth = 2;
        gbc.anchor = GridBagConstraints.CENTER;
        formPanel.add(loginButton, gbc);

        // Add Forgot Password button below the login button
        gbc.gridy = 4;
        gbc.anchor = GridBagConstraints.CENTER;
        formPanel.add(forgotPasswordButton, gbc);

        // ActionListener for login button
        loginButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String username = userField.getText();
                String password = new String(passField.getPassword());

                if (validateLogin(username, password)) {
                    JOptionPane.showMessageDialog(LoginPage.this, "Login Successful");
                    dispose();
                    SwingUtilities.invokeLater(() -> new HospitalManagementSystem().setVisible(true));
                } else {
                    JOptionPane.showMessageDialog(LoginPage.this, "Invalid Username or Password", "Login Error", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        // ActionListener for Forgot Password button
        forgotPasswordButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                // Show a dialog asking for email or username
                String email = JOptionPane.showInputDialog(LoginPage.this, "Enter your registered email address:");

                if (email != null && !email.isEmpty()) {
                    if (isEmailRegistered(email)) {
                        JOptionPane.showMessageDialog(LoginPage.this, "Password reset link sent to your email.");
                    } else {
                        JOptionPane.showMessageDialog(LoginPage.this, "Email not found in our records.", "Error", JOptionPane.ERROR_MESSAGE);
                    }
                }
            }
        });

        backgroundPanel.add(formPanel, BorderLayout.CENTER);
        setContentPane(backgroundPanel);
    }

    private JButton createStyledButton(String text) {
        JButton button = new JButton(text);
        button.setFont(new Font("Arial", Font.BOLD, 16));
        button.setForeground(Color.BLACK);  // Set text color to black
        button.setBackground(Color.WHITE);  // Set background color to white
        button.setFocusPainted(false);
        button.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(Color.BLACK, 3),  // Black border
                BorderFactory.createEmptyBorder(5, 15, 5, 15)
        ));
        return button;
    }

    private boolean validateLogin(String username, String password) {
        String query = "SELECT * FROM users WHERE username = ? AND password = ?";
        try (PreparedStatement preparedStatement = connection.prepareStatement(query)) {
            preparedStatement.setString(1, username);
            preparedStatement.setString(2, password);
            try (ResultSet resultSet = preparedStatement.executeQuery()) {
                return resultSet.next();
            }
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
    }

    private boolean isEmailRegistered(String email) {
        // You can replace this logic with actual database check for the email
        String query = "SELECT * FROM users WHERE email = ?";
        try (PreparedStatement preparedStatement = connection.prepareStatement(query)) {
            preparedStatement.setString(1, email);
            try (ResultSet resultSet = preparedStatement.executeQuery()) {
                return resultSet.next();  // Return true if the email exists
            }
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
    }

    // Custom JPanel class to handle background image
    class BackgroundPanel extends JPanel {
        private Image backgroundImage;

        public BackgroundPanel(String filePath) {
            backgroundImage = new ImageIcon(filePath).getImage();
        }

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            // Draw the background image to fill the entire panel
            g.drawImage(backgroundImage, 0, 0, getWidth(), getHeight(), this);
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new LoginPage().setVisible(true));
    }
}

