# CICD
Development of a simple CI/CD for the SimpleBashUtils project. Building, testing, deployment.

### **CI/CD** basics

**CI/CD** is a set of principles and practices that enable more frequent and secure deployment of software changes.

Reasons for using **CI/CD**:
- Team development
- Long software life cycle
- Shortened release cycle
- Difficulties in deployment and testing of large systems
- Human factor

**CI/CD** pipeline is a sequence of actions (scripts) for a particular version of the code in the repository,
which is started automatically when changes are made.

### **CI** basics

**CI** (Continuous Integration) refers to the integration of individual pieces of application code with each other.
**CI** normally performs two tasks as described below.

- BUILD
    - Checking if the code is being built at all
    - Prepare the artifacts for the next stages
- TEST
    - Codestyle tests
    - Unit Tests
    - Integration tests
    - Other tests you have
    - Test reports

### **CD** basics

**CD** (Continuous Delivery) is a continuous integration extension, as it automatically deploys all code changes to the test and/or production environment after the build stage.
**CD** can perform the following tasks.

- PUBLISH (If using a deployment docker)
    - Build container images
    - Push the images to where they will be taken from for deployment
- UPDATE CONFIGS
    - Update configuration on the machines
- DEPLOY STAGING
    - Deployment of test environment for manual tests, QA, and other non-automated checks
    - Can be run manually or automatically if CI stages are passed successfully
- DEPLOY PRODUCTION
    - Deploying a new version of the system on 'production'
    - This stage better be run manually rather than automatically
    - If you want, you can set it up for a specific branch of the repository only (master, release, etc.)

`-` There you go. If you have any questions, run what I said slowly through your head. I'll be right back.
