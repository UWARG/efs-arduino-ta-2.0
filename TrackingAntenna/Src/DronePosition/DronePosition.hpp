class DronePosition {
   private:
    static float latitude_;
    static float longitude_;
    static float altitude_;

   public:
    static void getPosition(void * pvParameters);
    static float latitude();
    static float longitude();
    static float altitude();
};