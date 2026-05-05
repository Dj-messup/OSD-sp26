# Security Application Write-Up: Secure Access Data Portal

## Purpose

This write-up demonstrates an application in security using my Secure Access Data Portal project.

https://github.com/Dj-messup/Secure-Access-Data-Portal/tree/main

The project was originally built as a final project for another class, but for this assignment I am focusing specifically on the security techniques used in the application. The main security goal is to protect sensitive hospital-style records by controlling who can log in, who can access records, and what information is shown to different users.

## Project Overview

The Secure Access Data Portal is a healthcare-style security application. It is designed around a hospital portal where users may need access to sensitive patient or provider information.

A basic username and password system is not enough for this kind of application because medical-style records need stronger protection. The system demonstrates several security controls working together instead of relying on only one layer of defense.

The main security techniques are:

1. Username and password authentication
2. Multi-factor authentication
3. Role-based access control
4. Audit logging
5. Data masking
6. Tokenization
7. Protected record access

## Security Problem

The security problem is unauthorized access to sensitive records.

In a hospital-style system, not every user should have the same access. A patient, provider, nurse, doctor, admin, and auditor may all need different permissions. If everyone can access the same records or view the same amount of information, the system creates a privacy and security risk.

The project addresses this by checking both identity and authorization.

Authentication answers:

Who is trying to log in?

Authorization answers:

What is this user allowed to do?

## Multi-Factor Authentication

The first major security feature is multi-factor authentication.

After a user logs in with a username and password, the system requires an additional verification step. This means that a stolen password alone should not be enough to fully access the system.

The project supports MFA behavior through interchangeable MFA strategies. For example, the system can use an email-based one-time password strategy or a backup TOTP-style strategy for demo purposes.

This is important because MFA adds another layer of security after the password step.

## Role-Based Access Control

The second major security feature is role-based access control.

RBAC limits what users can do based on their assigned role. Instead of hardcoding one access rule for every user, the system checks the user's role and permission before allowing access to protected features.

For example, an admin-style user may be allowed to review audit logs, while a normal patient account should not have that same permission. A provider may be allowed to view protected records, while another role may only be allowed to view masked information.

This protects the system from giving every user the same level of access.

## Audit Logging

The third major security feature is audit logging.

Audit logs are important because security is not only about blocking access. A secure system also needs visibility into what happened.

The portal logs security-related events such as authentication activity, authorization checks, record access, and admin-style actions.

This matters because audit logs can help answer questions like:

1. Who tried to log in?
2. Was MFA completed?
3. Who accessed a protected record?
4. Was access allowed or denied?
5. What security events happened in the system?

Audit logging supports accountability and incident investigation.

## Data Masking

The project also demonstrates data masking.

Data masking hides part of a sensitive value so the full value is not exposed unless the user has the correct permission. This is useful for fields like identifiers, record numbers, or other sensitive information.

For example, instead of showing a full sensitive identifier, the system can show only a partial version. This reduces the chance of exposing private data to users who do not need the full value.

## Tokenization

The project also uses tokenization as a security technique.

Tokenization replaces a sensitive value with a token. The token can be used by the application without directly exposing the original sensitive value.

This is useful because the application can still refer to a record or identifier without showing the real sensitive data everywhere in the system.

## Layered Security Design

The project uses layered security instead of one single security feature.

The user must first authenticate. Then the user must pass MFA. After that, the system still checks the user's role before allowing access to protected records or audit logs.

This layered structure is important because one security control may fail or be bypassed. Using multiple controls makes the system stronger.

The basic flow is:

1. User submits username and password
2. System checks login credentials
3. System requires MFA verification
4. System checks the user's role and permissions
5. System allows or denies record access
6. System writes security events to the audit log
7. Sensitive information is masked or tokenized when needed

## Why This Is an Application in Security

This project is an application in security because it demonstrates real security techniques used in software systems that handle sensitive information.

It is not only a login page. It combines authentication, MFA, authorization, logging, and sensitive data protection.

The main idea is defense in depth. The system does not rely on only one protection. It uses multiple layers to reduce risk.

## What I Learned

This project helped me understand that security has to be built into the design of an application.

It is not enough to add a login page at the end. The application needs clear roles, protected routes, permission checks, secure handling of sensitive data, and logs that show what happened.

I also learned that security code should be organized clearly. Separating routes, controllers, services, repositories, models, and security helpers makes the system easier to test and maintain.

## Conclusion

The Secure Access Data Portal demonstrates an application in security by protecting sensitive hospital-style records with multiple security controls.

The project uses multi-factor authentication to strengthen login, role-based access control to limit permissions, audit logging to track security events, and masking and tokenization to reduce exposure of sensitive data.

Overall, this project shows how software design and security work together to protect sensitive information.
